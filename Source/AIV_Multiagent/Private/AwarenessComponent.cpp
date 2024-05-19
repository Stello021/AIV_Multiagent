// Fill out your copyright notice in the Description page of Project Settings.

#include "AwarenessComponent.h"
#include "AlertComponent.h"
#include "AI_BaseController.h"
#include "AI_BaseNPC.h"
#include "AIInfo_DataAsset.h"

// Sets default values for this component's properties
UAwarenessComponent::UAwarenessComponent()
	: Super()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bAutoActivate = true;
	bWantsInitializeComponent = true;

	CurrentAwarenessValue = 0.f;
	CanDecrease = true;
	IsDecreasing = false;

	CanUpdateAwareness = true;
}

void UAwarenessComponent::InitializeComponent()
{
	AICRef = Cast<AAI_BaseController>(GetOwner());

	Super::InitializeComponent();

}

// Called when the game starts
void UAwarenessComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAwarenessComponent::SetupAwarenessComponent()
{
	if (IsValid(AICRef) && IsValid(AICRef->GetNPCRef()))
	{
		MaxAwarenessValue = AICRef->GetNPCRef()->AIInfo_DataAsset->MaxAwareness;
		AwarenessPauseTime = AICRef->GetNPCRef()->AIInfo_DataAsset->AwarenessPauseTime;
		AwarenessDecreaseTime = AICRef->GetNPCRef()->AIInfo_DataAsset->AwarenessDecreaseTime;
		AwarenessDecreaseValue = AICRef->GetNPCRef()->AIInfo_DataAsset->AwarenessDecreaseValue;
		AwarenessIncreaseOnSight_Narrow = AICRef->GetNPCRef()->AIInfo_DataAsset->AwarenessIncreaseOnSight_Narrow;
		AwarenessIncreaseOnSight_Wide = AICRef->GetNPCRef()->AIInfo_DataAsset->AwarenessIncreaseOnSight_Wide;
		AwarenessIncreaseOnSight_Peripheral = AICRef->GetNPCRef()->AIInfo_DataAsset->AwarenessIncreaseOnSight_Peripheral;
		AwarenessIncreaseOnSight_Backward = AICRef->GetNPCRef()->AIInfo_DataAsset->AwarenessIncreaseOnSight_Backward;
		AwarenessIncreaseOnWhistle = AICRef->GetNPCRef()->AIInfo_DataAsset->AwarenessIncreaseOnWhistle;
		AwarenessIncreaseOnFootsteps = AICRef->GetNPCRef()->AIInfo_DataAsset->AwarenessIncreaseOnFootsteps;
	}
}

void UAwarenessComponent::UpdateAwarenessFromSense(E_AISense InputSense, bool IsWhistle, E_AISightCone CurrentTypeOfCone)
{
	switch (InputSense)
	{
		case E_AISense::SIGHT:
		{
			UpdateAwarenessValue(GetCorrectSightAwarenessValue(CurrentTypeOfCone));
			
			break;
		}

		case E_AISense::HEARING:
		{
			if(IsWhistle)
			{
				UpdateAwarenessValue(AwarenessIncreaseOnWhistle);
			}
			else
			{
				UpdateAwarenessValue(AwarenessIncreaseOnFootsteps);
			}

			break;
		}

		case E_AISense::TOUCH:
		{
			UpdateAwarenessValue(MaxAwarenessValue);

			break;
		}

		default:
		{
			break;
		}
	}
}

void UAwarenessComponent::UpdateAwarenessValue(float AwarenessDelta)
{
	if (IsValid(AICRef) && !AICRef->GetAlertComponent()->GetCanUpdateAlert())
	{
		if (AwarenessDelta > 0)
		{
			SetDecreasing(false);
			GetWorld()->GetTimerManager().ClearTimer(DecreaseTimerHandle);

			if (PauseTimerHandle.IsValid())
			{
				//Restart PauseTimer
				GetWorld()->GetTimerManager().ClearTimer(PauseTimerHandle);
				GetWorld()->GetTimerManager().SetTimer(PauseTimerHandle, this, &UAwarenessComponent::OnPauseFinished, AwarenessPauseTime, false);
			}
			else
			{
				//Set PauseTimer
				GetWorld()->GetTimerManager().SetTimer(PauseTimerHandle, this, &UAwarenessComponent::OnPauseFinished, AwarenessPauseTime, false);
			}
		}

		CurrentAwarenessValue = FMath::Clamp(CurrentAwarenessValue + AwarenessDelta, 0, MaxAwarenessValue);

		if (CurrentAwarenessValue == MaxAwarenessValue && GetCanUpdateAwareness())
		{
			CurrentAwarenessValue = MaxAwarenessValue;
			SetCanUpdateAwareness(false);

			// Stop Awareness Component
			StopAwareness();

			// Start Alert Component
			AICRef->GetAlertComponent()->SetCanUpdateAlert(true);

			AICRef->UpdateCurrentStatusTag(E_AITag::SUSPICIOUS);
		}
	}
}

float UAwarenessComponent::GetCorrectSightAwarenessValue(E_AISightCone SightConeType)
{
	switch (SightConeType)
	{
	case E_AISightCone::NARROW:
	{
		return AwarenessIncreaseOnSight_Narrow;
	}
	case E_AISightCone::WIDE:
	{
		return AwarenessIncreaseOnSight_Wide;
	}
	case E_AISightCone::PERIPHERAL:
	{
		return AwarenessIncreaseOnSight_Peripheral;
	}
	case E_AISightCone::BACKWARD:
	{
		return AwarenessIncreaseOnSight_Backward;
	}
	default:
	{
		return 0.f;
	}
	}
}

void UAwarenessComponent::OnPauseFinished()
{
	GetWorld()->GetTimerManager().ClearTimer(PauseTimerHandle);

	if (!GetIsDecreasing() && GetCanDecrease())
	{
		SetDecreasing(true);

		//Decrease first time and then decrease every X seconds (timer)
		OnDecreaseAwarenessValue();
		GetWorld()->GetTimerManager().ClearTimer(DecreaseTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(DecreaseTimerHandle, this, &UAwarenessComponent::OnDecreaseAwarenessValue, AwarenessDecreaseTime, true);
	}
}

void UAwarenessComponent::OnDecreaseAwarenessValue()
{
	if (GetIsDecreasing())
	{
		if (CurrentAwarenessValue <= 0)
		{
			CurrentAwarenessValue = 0;
			ResetAwareness();
		}
		else
		{
			UpdateAwarenessValue(-AwarenessDecreaseValue);

			if (CurrentAwarenessValue <= 0)
			{
				OnDecreaseAwarenessValue();
			}
		}
	}
}

void UAwarenessComponent::ResetAwareness()
{
	StopAwareness();

	CurrentAwarenessValue = 0.f;
	AICRef->GetAlertComponent()->ResetAlert();
	AICRef->GetAlertComponent()->SetCanUpdateAlert(false);

	AICRef->ClearDisturbanceKey();
	AICRef->UpdateCurrentStatusTag(E_AITag::UNAWARE);

	SetCanUpdateAwareness(true);
}

void UAwarenessComponent::StopAwareness()
{
	SetDecreasing(false);
	GetWorld()->GetTimerManager().ClearTimer(PauseTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(DecreaseTimerHandle);
}
