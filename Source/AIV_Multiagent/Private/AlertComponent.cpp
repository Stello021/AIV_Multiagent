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
}

void UAlertComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

// Called when the game starts
void UAlertComponent::BeginPlay()
{
	Super::BeginPlay();
}