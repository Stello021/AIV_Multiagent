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

}

void UTensionMeterComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

// Called when the game starts
void UTensionMeterComponent::BeginPlay()
{
	Super::BeginPlay();
}