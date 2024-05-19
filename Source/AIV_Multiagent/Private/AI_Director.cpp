// Fill out your copyright notice in the Description page of Project Settings.

#include "AI_Director.h"
#include "AI_BaseNPC.h"
#include "AI_BaseController.h"
#include "AwarenessComponent.h"
#include "AlertComponent.h"
#include "TensionMeterComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AIDirector_DataAsset.h"

// Sets default values
AAI_Director::AAI_Director()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TensionMeterComponent = CreateDefaultSubobject<UTensionMeterComponent>(TEXT("Tension Meter Component"));
}

// Called when the game starts or when spawned
void AAI_Director::BeginPlay()
{
	SetupAIDirector();

	Super::BeginPlay();
}

void AAI_Director::SetupAIDirector()
{
	if (AIDirector_DataAsset)
	{
		UpdateDirectorFrequency = AIDirector_DataAsset->UpdateDirectorFrequency;
		CloseNPCThreshold = AIDirector_DataAsset->CloseNPCThreshold;
		EliteAlertRange = AIDirector_DataAsset->EliteAlertRange;
	}
}

void AAI_Director::StartUpdateDirector()
{
	GetWorld()->GetTimerManager().SetTimer(UpdateDirectorTimerHandle, this, &AAI_Director::UpdateDirectorInfo, UpdateDirectorFrequency, true);
}

void AAI_Director::AddLinkedNPC(AAI_BaseNPC* Caller, AAI_BaseNPC* Linked)
{
	FLinkedNPCStruct CurrentLinkedNPCs;

	if (LinkedNPCs.IsEmpty() || !LinkedNPCs.Contains(Caller))
	{
		CurrentLinkedNPCs.LinkedArray.AddUnique(Linked);
	}
	else
	{
		CurrentLinkedNPCs = *LinkedNPCs.Find(Caller);
		CurrentLinkedNPCs.LinkedArray.AddUnique(Linked);
	}

	LinkedNPCs.Add(Caller, CurrentLinkedNPCs);

	Linked->GetAICRef()->LinkToNPC(Caller);
}

void AAI_Director::RemoveLinkedNPC(AAI_BaseNPC* Caller, AAI_BaseNPC* Linked)
{
	FLinkedNPCStruct CurrentLinkedNPCs;

	if (LinkedNPCs.Contains(Caller))
	{
		CurrentLinkedNPCs = *LinkedNPCs.Find(Caller);
		CurrentLinkedNPCs.LinkedArray.Remove(Linked);

		if (!CurrentLinkedNPCs.LinkedArray.IsEmpty())
		{
			for (AAI_BaseNPC* CurrentNPCRef : CurrentLinkedNPCs.LinkedArray)
			{
				if (CurrentNPCRef->GetAICRef()->CheckCurrentStatusTag(E_AITag::UNAWARE))
				{
					CurrentLinkedNPCs.LinkedArray.Remove(CurrentNPCRef);
				}
			}
		}

		LinkedNPCs.Add(Caller, CurrentLinkedNPCs);

		if (CurrentLinkedNPCs.LinkedArray.IsEmpty())
		{
			Caller->GetAICRef()->GetAlertComponent()->ResetAlert();
			Caller->GetAICRef()->GetAwarenessComponent()->ResetAwareness();
		}
	}
}

void AAI_Director::UpdateDirectorInfo()
{
	if (TensionMeterComponent->GetCurrentTensionValue() > TensionMeterComponent->GetTensionThreshold())
	{
		MoveEliteAwayFromPlayer();
	}
	else
	{
		MoveEliteCloseToPlayer();
	}
}

int32 AAI_Director::GetCloseNPCs()
{
	int32 CurrentCloseNPCs = 0;

	if (!NPCList.IsEmpty())
	{
		for (AAI_BaseNPC* EvaluatedNPC : NPCList)
		{
			double DistanceFromPlayer = ((UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->GetActorLocation() - EvaluatedNPC->GetActorLocation()).Size();
			if (DistanceFromPlayer <= CloseNPCThreshold)
			{
				CurrentCloseNPCs++;
			}
		}
	}

	return CurrentCloseNPCs;
}

void AAI_Director::MoveEliteAwayFromPlayer_Implementation()
{

}

void AAI_Director::MoveEliteCloseToPlayer_Implementation()
{

}