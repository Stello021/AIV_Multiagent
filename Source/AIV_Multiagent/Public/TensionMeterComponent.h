// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TensionMeterComponent.generated.h"

class AAI_Director;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AIV_MULTIAGENT_API UTensionMeterComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTensionMeterComponent();

	virtual void InitializeComponent() override;

	void SetupTensionMeterComponent();

	UFUNCTION(BlueprintCallable, Category = "TensionMeter")
	void UpdateTension();

	UFUNCTION(BlueprintPure, Category = "TensionMeter")
	int32 GetCurrentTensionValue() const { return CurrentTensionValue; }

	UFUNCTION(BlueprintCallable, Category = "TensionMeter")
	void UpdateTensionValue(int32 UpdatedTension);

	UFUNCTION(BlueprintPure, Category = "TensionMeter")
	int32 GetTensionThreshold() const { return TensionThreshold; }

	void StartTensionUpdate();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	AAI_Director* AIDirectorRef;

	int32 CurrentTensionValue;
	int32 MaxTension;
	int32 TensionDecay;
	int32 TensionThreshold;
	int32 TensionNPCIncrement;

	/* Handle to manage the timer */
	FTimerHandle UpdateTensionTimerHandle;
	float UpdateTensionFrequency;
};
