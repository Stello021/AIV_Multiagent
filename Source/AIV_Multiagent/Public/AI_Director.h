// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AI_Director.generated.h"

class AAI_BaseNPC;
class UTensionMeterComponent;
class UAIDirector_DataAsset;

USTRUCT(BlueprintType)
struct AIV_MULTIAGENT_API FLinkedNPCStruct
{
	GENERATED_BODY()

public:

	FLinkedNPCStruct() { LinkedArray = TArray<AAI_BaseNPC*>{}; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LinkedNPC")
	TArray<AAI_BaseNPC*> LinkedArray;
};

UCLASS()
class AIV_MULTIAGENT_API AAI_Director : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAI_Director();

	void SetupAIDirector();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DataAssets")
	UAIDirector_DataAsset* AIDirector_DataAsset;

	UFUNCTION(BlueprintPure, Category = "AIDirector")
	TArray<AAI_BaseNPC*> GetNPCList() const { return NPCList; }

	UFUNCTION(BlueprintCallable, Category = "AIDirector")
	void AddNPCToDirectorList(AAI_BaseNPC* NewNPC) { NPCList.AddUnique(NewNPC); }

	UFUNCTION(BlueprintPure, Category = "TensionMeter")
	UTensionMeterComponent* GetTensionMeterComponent() const { return TensionMeterComponent; }

	void StartUpdateDirector();

	void UpdateDirectorInfo();

	int32 GetCloseNPCs();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void MoveEliteAwayFromPlayer();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void MoveEliteCloseToPlayer();

	UFUNCTION(BlueprintCallable)
	void AddLinkedNPC(AAI_BaseNPC* Caller, AAI_BaseNPC* Linked);

	UFUNCTION(BlueprintPure, Category = "LinkedNPCs")
	TMap<AAI_BaseNPC*, FLinkedNPCStruct> GetLinkedNPCs() const { return LinkedNPCs; }

	UFUNCTION(BlueprintCallable)
	void RemoveLinkedNPC(AAI_BaseNPC* Caller, AAI_BaseNPC* Linked);

	UFUNCTION(BlueprintPure, Category = "AIDirector")
	float GetEliteAlertRange() const { return EliteAlertRange; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TArray<AAI_BaseNPC*> NPCList;

	UPROPERTY(Category = "TensionMeter", BlueprintReadOnly, EditAnywhere)
	TObjectPtr<UTensionMeterComponent> TensionMeterComponent = nullptr;

	/* Handle to manage the timer */
	FTimerHandle UpdateDirectorTimerHandle;
	float UpdateDirectorFrequency;

	float CloseNPCThreshold;
	float EliteAlertRange;

	TMap<AAI_BaseNPC*, FLinkedNPCStruct> LinkedNPCs;
};
