// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enum.h"
#include "AwarenessComponent.generated.h"

class AAI_BaseController;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AIV_MULTIAGENT_API UAwarenessComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAwarenessComponent();

	virtual void InitializeComponent() override;

	void SetupAwarenessComponent();

	UFUNCTION(Category = Default, BlueprintPure)
	float GetCurrentAwarenessValue() const { return CurrentAwarenessValue; }

	UFUNCTION(Category = Default, BlueprintPure)
	float GetMaxAwarenessValue() const { return MaxAwarenessValue; }

	UFUNCTION(Category = Default, BlueprintCallable)
	void UpdateAwarenessFromSense(E_AISense InputSense, bool IsWhistle = false, E_AISightCone CurrentTypeOfCone = E_AISightCone::NONE);

	UFUNCTION(Category = Default, BlueprintCallable)
	void UpdateAwarenessValue(float AwarenessDelta);

	void OnDecreaseAwarenessValue();

	UFUNCTION(Category = Default, BlueprintCallable)
	void ResetAwareness();

	void StopAwareness();

	void SetDecreasing(bool NewStatus) { IsDecreasing = NewStatus; }

	bool GetIsDecreasing() const { return IsDecreasing; }

	void SetCanDecrease(bool NewStatus) { CanDecrease = NewStatus; }

	bool GetCanDecrease() const { return CanDecrease; }

	void SetCanUpdateAwareness(bool NewStatus) { CanUpdateAwareness = NewStatus; }

	bool GetCanUpdateAwareness() const { return CanUpdateAwareness; }

	UFUNCTION(Category = Default, BlueprintCallable)
	void OnPauseFinished();

	float GetCorrectSightAwarenessValue(E_AISightCone SightConeType);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	AAI_BaseController* AICRef;

	/* Handle to manage the timer */
	FTimerHandle PauseTimerHandle;

	/* Handle to manage the timer */
	FTimerHandle DecreaseTimerHandle;

private:

	float CurrentAwarenessValue;
	float MaxAwarenessValue;
	float AwarenessDecreaseValue;
	bool CanDecrease;
	bool IsDecreasing;
	float AwarenessPauseTime;
	float AwarenessDecreaseTime;
	float AwarenessIncreaseOnSight_Narrow;
	float AwarenessIncreaseOnSight_Wide;
	float AwarenessIncreaseOnSight_Peripheral;
	float AwarenessIncreaseOnSight_Backward;
	float AwarenessIncreaseOnWhistle;
	float AwarenessIncreaseOnFootsteps;

	bool CanUpdateAwareness;
};
