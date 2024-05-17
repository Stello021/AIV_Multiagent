// Fill out your copyright notice in the Description page of Project Settings.

#include "AI_BaseNPC.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI_BaseController.h"
#include "AIInfo_DataAsset.h"

// Sets default values
AAI_BaseNPC::AAI_BaseNPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAI_BaseNPC::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAI_BaseNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}