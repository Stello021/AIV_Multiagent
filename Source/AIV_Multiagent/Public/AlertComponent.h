// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enum.h"
#include "AlertComponent.generated.h"

class AAI_BaseController;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AIV_MULTIAGENT_API UAlertComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAlertComponent();

	virtual void InitializeComponent() override;

	void SetupAlertComponent();

	UFUNCTION(Category = Default, BlueprintPure)
	float GetCurrentAlertValue() const { return CurrentAlertValue; }

	UFUNCTION(Category = Default, BlueprintPure)
	float GetMaxAlertValue() const { return MaxAlertValue; }

	UFUNCTION(Category = Default, BlueprintCallable)
	void UpdateAlertFromSense(E_AISense InputSense, E_AISightCone CurrentTypeOfCone = E_AISightCone::NONE);

	UFUNCTION(Category = Default, BlueprintCallable)
	void UpdateAlertValue(float AlertDelta, bool IsFromTouching = false);

	void OnDecreaseAlertValue();

	UFUNCTION(Category = Default, BlueprintCallable)
	void ResetAlert();

	void StopAlert();

	void SetDecreasing(bool NewStatus) { IsDecreasing = NewStatus; }

	bool GetIsDecreasing() const { return IsDecreasing; }

	void SetCanDecrease(bool NewStatus) { CanDecrease = NewStatus; }

	bool GetCanDecrease() const { return CanDecrease; }

	void SetCanUpdateAlert(bool NewStatus) { CanUpdateAlert = NewStatus; }

	bool GetCanUpdateAlert() const { return CanUpdateAlert; }

	UFUNCTION(Category = Default, BlueprintCallable)
	void OnPauseFinished();

	float GetCorrectSightAlertValue(E_AISightCone SightConeType);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	AAI_BaseController* AICRef;

	/* Handle to manage the timer */
	FTimerHandle PauseTimerHandle;

	/* Handle to manage the timer */
	FTimerHandle DecreaseTimerHandle;

private:

	float CurrentAlertValue;
	float MaxAlertValue;
	float AlertDecreaseValue;
	bool CanDecrease;
	bool IsDecreasing;
	float AlertPauseTime;
	float AlertDecreaseTime;
	float AlertIncreaseOnSight_Narrow;
	float AlertIncreaseOnSight_Wide;
	float AlertIncreaseOnSight_Peripheral;
	float AlertIncreaseOnSight_Backward;

	bool CanUpdateAlert;
};
