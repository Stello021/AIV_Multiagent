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

	AIType = E_AIType::REGULAR;
}

// Called when the game starts or when spawned
void AAI_BaseNPC::BeginPlay()
{
	AICRef = Cast<AAI_BaseController>(GetController());

	Super::BeginPlay();
}

// Called every frame
void AAI_BaseNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAI_BaseNPC::ChangeSpeedToSprint()
{
	if (CurrentSpeedType != E_AISpeed::RUN)
	{
		GetCharacterMovement()->MaxWalkSpeed = AIInfo_DataAsset->RunSpeed;
		CurrentSpeedType = E_AISpeed::RUN;
	}
}

void AAI_BaseNPC::ChangeSpeedToWalk()
{
	if (CurrentSpeedType != E_AISpeed::WALK)
	{
		GetCharacterMovement()->MaxWalkSpeed = AIInfo_DataAsset->WalkSpeed;
		CurrentSpeedType = E_AISpeed::WALK;
	}
}