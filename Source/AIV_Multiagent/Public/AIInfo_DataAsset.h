// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AIInfo_DataAsset.generated.h"

/**
 * 
 */
UCLASS()
class AIV_MULTIAGENT_API UAIInfo_DataAsset : public UDataAsset
{
	GENERATED_BODY()
	

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float WalkSpeed = 230.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RunSpeed = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RotationSpeed = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Awareness|Setup")
	float MaxAwareness = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Awareness|Setup")
	float AwarenessPauseTime = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Awareness|Setup")
	float AwarenessDecreaseTime = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Awareness|Setup")
	float AwarenessDecreaseValue = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Awareness|Sight")
	float AwarenessFrequencyUpdateOnSight = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Awareness|Sight")
	float AwarenessIncreaseOnSight_Narrow = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Awareness|Sight")
	float AwarenessIncreaseOnSight_Wide = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Awareness|Sight")
	float AwarenessIncreaseOnSight_Peripheral = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Awareness|Sight")
	float AwarenessIncreaseOnSight_Backward = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Awareness|Hearing")
	float AwarenessIncreaseOnFootsteps = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Awareness|Hearing")
	float AwarenessIncreaseOnWhistle = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Alert|Setup")
	float MaxAlertValue = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Alert|Setup")
	float AlertDecreaseValue = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Alert|Setup")
	float AlertPauseTime = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Alert|Setup")
	float AlertDecreaseTime = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Alert|Sight")
	float AlertIncreaseOnSight_Narrow = 2.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Alert|Sight")
	float AlertIncreaseOnSight_Wide = 1.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Alert|Sight")
	float AlertIncreaseOnSight_Peripheral = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Alert|Sight")
	float AlertIncreaseOnSight_Backward = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Alert|Sight")
	float AlertFrequencyUpdateOnSight = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception|Sight")
	float SightRadius = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception|Sight")
	float LoseSightRadius = 700.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception|Sight")
	float PeripheralVisionAngleDegrees = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception|Sight")
	float LoseSightTimer = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception|Sight")
	float SightPeripheralHalfAngleDegree_Narrow = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception|Sight")
	float SightRadius_Wide = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception|Sight")
	float SightPeripheralHalfAngleDegree_Wide = 45.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception|Sight")
	float SightRadius_Peripheral = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception|Sight")
	float SightPeripheralHalfAngleDegree_Peripheral = 60.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception|Sight")
	float SightRadius_Backward = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception|Hearing")
	float RunHearingRange = 750.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception|Hearing")
	float WalkHearingRange = 250.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BT|Investigation")
	float InvestigationTime = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BT|Investigation")
	float DisturbanceCooldown = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BT|Hunting")
	float HuntingTime = 20.f;
};
