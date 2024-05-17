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
}

// Called when the game starts or when spawned
void AAI_Director::BeginPlay()
{
	Super::BeginPlay();
}