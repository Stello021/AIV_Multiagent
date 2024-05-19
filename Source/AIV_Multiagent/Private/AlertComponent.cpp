// Fill out your copyright notice in the Description page of Project Settings.

#include "AlertComponent.h"
#include "AI_BaseController.h"
#include "AI_BaseNPC.h"
#include "AIInfo_DataAsset.h"

// Sets default values for this component's properties
UAlertComponent::UAlertComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bAutoActivate = true;
	bWantsInitializeComponent = true;

	CurrentAlertValue = 0.f;
	CanDecrease = true;
	IsDecreasing = false;

	CanUpdateAlert = false;
}

void UAlertComponent::InitializeComponent()
{
	AICRef = Cast<AAI_BaseController>(GetOwner());

	Super::InitializeComponent();
}

// Called when the game starts
void UAlertComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAlertComponent::SetupAlertComponent()
{
	if (IsValid(AICRef) && IsValid(AICRef->GetNPCRef()))
	{
		MaxAlertValue = AICRef->GetNPCRef()->AIInfo_DataAsset->MaxAlertValue;
		AlertDecreaseValue = AICRef->GetNPCRef()->AIInfo_DataAsset->AlertDecreaseValue;
		AlertPauseTime = AICRef->GetNPCRef()->AIInfo_DataAsset->AlertPauseTime;
		AlertDecreaseTime = AICRef->GetNPCRef()->AIInfo_DataAsset->AlertDecreaseTime;
		AlertIncreaseOnSight_Narrow = AICRef->GetNPCRef()->AIInfo_DataAsset->AlertIncreaseOnSight_Narrow;
		AlertIncreaseOnSight_Wide = AICRef->GetNPCRef()->AIInfo_DataAsset->AlertIncreaseOnSight_Wide;
		AlertIncreaseOnSight_Peripheral = AICRef->GetNPCRef()->AIInfo_DataAsset->AlertIncreaseOnSight_Peripheral;
		AlertIncreaseOnSight_Backward = AICRef->GetNPCRef()->AIInfo_DataAsset->AlertIncreaseOnSight_Backward;
	}
}

void UAlertComponent::UpdateAlertFromSense(E_AISense InputSense, E_AISightCone CurrentTypeOfCone)
{
	switch (InputSense)
	{
	case E_AISense::SIGHT:
	{
		UpdateAlertValue(GetCorrectSightAlertValue(CurrentTypeOfCone));

		break;
	}

	case E_AISense::HEARING:
	{
		break;
	}

	case E_AISense::TOUCH:
	{
		UpdateAlertValue(MaxAlertValue, true);

		break;
	}

	default:
	{
		break;
	}
	}
}

void UAlertComponent::UpdateAlertValue(float AlertDelta, bool IsFromTouching)
{
	if (IsValid(AICRef) && GetCanUpdateAlert())
	{
		if (AlertDelta > 0)
		{
			SetDecreasing(false);
			GetWorld()->GetTimerManager().ClearTimer(DecreaseTimerHandle);

			if (PauseTimerHandle.IsValid())
			{
				//Restart PauseTimer
				GetWorld()->GetTimerManager().ClearTimer(PauseTimerHandle);
				GetWorld()->GetTimerManager().SetTimer(PauseTimerHandle, this, &UAlertComponent::OnPauseFinished, AlertPauseTime, false);
			}
			else
			{
				//Set PauseTimer
				GetWorld()->GetTimerManager().SetTimer(PauseTimerHandle, this, &UAlertComponent::OnPauseFinished, AlertPauseTime, false);
			}
		}

		CurrentAlertValue = FMath::Clamp(CurrentAlertValue + AlertDelta, 0, MaxAlertValue);

		if (CurrentAlertValue == MaxAlertValue && GetCanUpdateAlert())
		{
			CurrentAlertValue = MaxAlertValue;
			SetCanUpdateAlert(false);

			StopAlert();

			AICRef->UpdateCurrentStatusTag(IsFromTouching ? E_AITag::HUNTING : E_AITag::ALERTED);
		}
	}
}

float UAlertComponent::GetCorrectSightAlertValue(E_AISightCone SightConeType)
{
	switch (SightConeType)
	{
	case E_AISightCone::NARROW:
	{
		return AlertIncreaseOnSight_Narrow;
	}
	case E_AISightCone::WIDE:
	{
		return AlertIncreaseOnSight_Wide;
	}
	case E_AISightCone::PERIPHERAL:
	{
		return AlertIncreaseOnSight_Peripheral;
	}
	case E_AISightCone::BACKWARD:
	{
		return AlertIncreaseOnSight_Backward;
	}
	default:
	{
		return 0.f;
	}
	}
}

void UAlertComponent::OnPauseFinished()
{
	GetWorld()->GetTimerManager().ClearTimer(PauseTimerHandle);

	if (!GetIsDecreasing() && GetCanDecrease())
	{
		SetDecreasing(true);

		//Decrease first time and then decrease every X seconds (timer)
		OnDecreaseAlertValue();
		GetWorld()->GetTimerManager().ClearTimer(DecreaseTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(DecreaseTimerHandle, this, &UAlertComponent::OnDecreaseAlertValue, AlertDecreaseTime, true);
	}
}

void UAlertComponent::OnDecreaseAlertValue()
{
	if (GetIsDecreasing())
	{
		if (CurrentAlertValue <= 0)
		{
			CurrentAlertValue = 0;
			ResetAlert();
		}
		else
		{
			UpdateAlertValue(-AlertDecreaseValue);

			if (CurrentAlertValue <= 0)
			{
				OnDecreaseAlertValue();
			}
		}
	}
}

void UAlertComponent::ResetAlert()
{
	StopAlert();
	CurrentAlertValue = 0.f;

	AICRef->UpdateCurrentStatusTag(E_AITag::SUSPICIOUS);
}

void UAlertComponent::StopAlert()
{
	SetDecreasing(false);
	GetWorld()->GetTimerManager().ClearTimer(PauseTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(DecreaseTimerHandle);
}
