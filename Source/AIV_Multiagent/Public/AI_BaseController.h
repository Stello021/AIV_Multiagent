// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Enum.h"
#include "GenericTeamAgentInterface.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "GameplayTagContainer.h"
#include "AI_BaseController.generated.h"

class UBehaviorTree;
class UBehaviorTreeComponent;
class UAwarenessComponent;
class UAlertComponent;
class AAI_BaseNPC;
class AAI_Director;

UCLASS(BlueprintType)
class AIV_MULTIAGENT_API AAI_BaseController : public AAIController
{
	GENERATED_BODY()
	
public:

	AAI_BaseController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintPure, Category = "NPC")
	AAI_BaseNPC* GetNPCRef() { return NPCRef; }

	UFUNCTION(BlueprintPure, Category = "Awareness")
	UAwarenessComponent* GetAwarenessComponent() const { return AwarenessComponent; }

	UFUNCTION(BlueprintPure, Category = "Awareness")
	UAlertComponent* GetAlertComponent() const { return AlertComponent; }

	UFUNCTION(BlueprintCallable)
	void RunBT();

	UFUNCTION(BlueprintCallable)
	void SetupSightInfo();

	UFUNCTION(BlueprintCallable)
	void SetupHearingInfo();

	UFUNCTION()
	void ActorPerceivedUpdate(AActor* UpdatedActor, FAIStimulus Stimulus);

	/******* TEAM ID ******/
	ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	/******* START STATUS TAGS ******/
	UFUNCTION(BlueprintPure, Category = "AIC|Status")
	FGameplayTag GetCurrentStatusTag() const { return CurrentStatusTag; }

	UFUNCTION(BlueprintCallable, Category = "AIC|Status")
	void UpdateCurrentStatusTag(E_AITag NewTag);

	UFUNCTION(BlueprintPure, Category = "AIC|Status")
	bool CheckCurrentStatusTag(E_AITag TagToCheck);
	/******* END STATUS TAGS ******/

	/******* START BB KEYS ******/
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Blackboard Key")
	FName KeyNameDisturbanceLocation = "DisturbanceLocation";

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Blackboard Key")
	FName KeyNameTarget = "Target";

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Blackboard Key")
	FName KeyNameIsTargetOnSight = "IsTargetOnSight";

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Blackboard Key")
	FName KeyNameHasTouchedTarget = "HasTouchedTarget";

	UFUNCTION(BlueprintCallable, Category = "Blackboard Key")
	void ClearDisturbanceKey();

	UFUNCTION(BlueprintCallable, Category = "Blackboard Key")
	void ClearTargetKey();
	/******* END BB KEYS ******/

	UFUNCTION(BlueprintCallable)
	E_AISightCone GetTypeOfSightCone(AActor* UpdatedActor);

	UFUNCTION(BlueprintCallable, Category = "AIC|Status")
	void SetHasCatchedTarget(bool UpdateStatus) { HasCatchedTarget = UpdateStatus; }

	UFUNCTION(BlueprintPure, Category = "AIC|Status")
	bool GetHasCatchedTarget() const { return HasCatchedTarget; }

	// DIRECTOR

	UFUNCTION(BlueprintPure, Category = "AI Director")
	AAI_Director* GetAIDirector() const { return AIDirectorRef; }

	UFUNCTION(BlueprintCallable, Category = "AI Director")
	void SetupAIDirector();

	UFUNCTION(BlueprintCallable, Category = "LinkedNPCs")
	void LinkToNPC(AAI_BaseNPC* Caller);

	UFUNCTION(BlueprintCallable, Category = "LinkedNPCs")
	void RemoveLinkToNPC();

	UFUNCTION(BlueprintCallable, Category = "LinkedNPCs")
	void SetSuspiciousAsLinkedNPC();

	UFUNCTION(BlueprintCallable, Category = "LinkedNPCs")
	void SetHuntingAsLinkedNPC();

protected:

	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UAIPerceptionComponent* AIPerceptionComp;

	UPROPERTY(Category = "Awareness", BlueprintReadOnly, EditAnywhere)
	TObjectPtr<UAwarenessComponent> AwarenessComponent = nullptr;

	UPROPERTY(Category = "Alert", BlueprintReadOnly, EditAnywhere)
	TObjectPtr<UAlertComponent> AlertComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BT", meta = (ToolTip = "Set Behavior Tree to run"))
	UBehaviorTree* MainBehaviorTree;

	UPROPERTY(BlueprintReadOnly)
	UBehaviorTreeComponent* BrainTree;

	AAI_BaseNPC* NPCRef;

	FGameplayTag CurrentStatusTag;

	bool IsDisturbanceCooldownActive;

	/* Handle to manage the timer */
	FTimerHandle DisturbanceCooldownTimerHandle;

	UFUNCTION()
	void OnDisturbanceCooldownFinished();

	bool HasLoseSight;

	/* Handle to manage the timer */
	FTimerHandle LoseSightOnAlertedTimerHandle;

	UFUNCTION()
	void OnLoseSightTimerFinished();

	bool HasCatchedTarget;

	AAI_Director* AIDirectorRef;

	AAI_BaseNPC* LinkedNPC;
};
