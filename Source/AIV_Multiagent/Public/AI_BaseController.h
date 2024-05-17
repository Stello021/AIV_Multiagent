// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Enum.h"
#include "GenericTeamAgentInterface.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "GameplayTagContainer.h"
#include "AI_BaseController.generated.h"

UCLASS(BlueprintType)
class AIV_MULTIAGENT_API AAI_BaseController : public AAIController
{
	GENERATED_BODY()
	
public:

	AAI_BaseController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:

	virtual void OnPossess(APawn* InPawn) override;

};
