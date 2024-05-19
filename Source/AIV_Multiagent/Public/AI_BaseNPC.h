// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enum.h"
#include "GameFramework/Character.h"
#include "AI_BaseNPC.generated.h"

class UAIInfo_DataAsset;
class AAI_BaseController;

UCLASS()
class AIV_MULTIAGENT_API AAI_BaseNPC : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAI_BaseNPC();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure, Category = "AI Type")
	E_AIType GetAIType() const { return AIType; }

	UFUNCTION(BlueprintPure, Category = "AIC")
	AAI_BaseController* GetAICRef() const { return AICRef; }

	UFUNCTION(BlueprintCallable, Category = "Speed")
	void ChangeSpeedToSprint();

	UFUNCTION(BlueprintCallable, Category = "Speed")
	void ChangeSpeedToWalk();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DataAssets")
	UAIInfo_DataAsset* AIInfo_DataAsset;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "AI Type")
	E_AIType AIType;

	AAI_BaseController* AICRef;

private:

	E_AISpeed CurrentSpeedType;
};
