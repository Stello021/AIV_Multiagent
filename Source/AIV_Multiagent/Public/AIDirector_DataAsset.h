// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AIDirector_DataAsset.generated.h"

/**
 * 
 */
UCLASS()
class AIV_MULTIAGENT_API UAIDirector_DataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Director")
	float UpdateDirectorFrequency = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Director")
	float CloseNPCThreshold = 700.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Director")
	float EliteAlertRange = 2000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tension Meter")
	float UpdateTensionFrequency = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tension Meter", meta = (ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100"))
	int32 MaxTension = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tension Meter", meta = (ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100"))
	int32 TensionThreshold = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tension Meter", meta = (ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100"))
	int32 TensionNPCIncrement = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tension Meter", meta = (ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100"))
	int32 TensionDecay = 1;
	
};
