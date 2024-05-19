// Fill out your copyright notice in the Description page of Project Settings.

#include "TensionMeterComponent.h"
#include "AI_Director.h"
#include "AIDirector_DataAsset.h"

// Sets default values for this component's properties
UTensionMeterComponent::UTensionMeterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bAutoActivate = true;
	bWantsInitializeComponent = true;

	CurrentTensionValue = 0;
}

void UTensionMeterComponent::InitializeComponent()
{
	AIDirectorRef = Cast<AAI_Director>(GetOwner());

	Super::InitializeComponent();
}

// Called when the game starts
void UTensionMeterComponent::BeginPlay()
{
	Super::BeginPlay();

	SetupTensionMeterComponent();
}

void UTensionMeterComponent::StartTensionUpdate()
{
	GetWorld()->GetTimerManager().SetTimer(UpdateTensionTimerHandle, this, &UTensionMeterComponent::UpdateTension, UpdateTensionFrequency, true);
}

void UTensionMeterComponent::SetupTensionMeterComponent()
{
	if (IsValid(AIDirectorRef) && AIDirectorRef->AIDirector_DataAsset)
	{
		UpdateTensionFrequency = AIDirectorRef->AIDirector_DataAsset->UpdateTensionFrequency;
		MaxTension = AIDirectorRef->AIDirector_DataAsset->MaxTension;
		TensionThreshold = AIDirectorRef->AIDirector_DataAsset->TensionThreshold;
		TensionNPCIncrement = AIDirectorRef->AIDirector_DataAsset->TensionNPCIncrement;
		TensionDecay = AIDirectorRef->AIDirector_DataAsset->TensionDecay;

		// Start Timers
		StartTensionUpdate();
		AIDirectorRef->StartUpdateDirector();
	}
}

void UTensionMeterComponent::UpdateTension()
{
	if (IsValid(AIDirectorRef))
	{
		int32 CloseNPCs = AIDirectorRef->GetCloseNPCs();
		int32 NewTension = (CloseNPCs > 0 ? (CloseNPCs * TensionNPCIncrement) : -TensionDecay);
		UpdateTensionValue(NewTension);
	}
}

void UTensionMeterComponent::UpdateTensionValue(int32 UpdatedTension)
{
	CurrentTensionValue += UpdatedTension;
	CurrentTensionValue = FMath::Clamp(CurrentTensionValue, 0, MaxTension);
}
