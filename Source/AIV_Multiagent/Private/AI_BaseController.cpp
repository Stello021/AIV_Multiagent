// Fill out your copyright notice in the Description page of Project Settings.

#include "AI_BaseController.h"

#include "Perception/AIPerceptionComponent.h"
#include "AI_BaseNPC.h"
#include "AI_Director.h"
#include "AwarenessComponent.h"
#include "AlertComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIInfo_DataAsset.h"
#include "Enum.h"
#include "GameplayTagsManager.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Touch.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AAI_BaseController::AAI_BaseController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent"))) 
{
	Cast<UCrowdFollowingComponent>(GetPathFollowingComponent())->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::High);
}

void AAI_BaseController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}