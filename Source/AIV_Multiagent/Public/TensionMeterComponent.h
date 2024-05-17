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

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

};
