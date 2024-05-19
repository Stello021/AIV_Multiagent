// Fill out your copyright notice in the Description page of Project Settings.

#include "AI_BaseController.h"

#include "Perception/AIPerceptionComponent.h"
#include "AI_BaseNPC.h"
#include "AI_Director.h"
#include "AwarenessComponent.h"
#include "AlertComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIInfo_DataAsset.h"
#include "Enum.h"
#include "GameplayTagsManager.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Touch.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AAI_BaseController::AAI_BaseController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
	Cast<UCrowdFollowingComponent>(GetPathFollowingComponent())->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::High);

	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>("AI Perception Component");

	SetGenericTeamId(FGenericTeamId(1));

	AwarenessComponent = CreateDefaultSubobject<UAwarenessComponent>(TEXT("Awareness Component"));

	AlertComponent = CreateDefaultSubobject<UAlertComponent>(TEXT("Alert Component"));
}

void AAI_BaseController::OnPossess(APawn* InPawn)
{
	NPCRef = Cast<AAI_BaseNPC>(InPawn);

	SetupSightInfo();
	SetupHearingInfo();

	GetAwarenessComponent()->SetupAwarenessComponent();
	GetAlertComponent()->SetupAlertComponent();

	SetupAIDirector();

	Super::OnPossess(InPawn);

	AIPerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &AAI_BaseController::ActorPerceivedUpdate);

	UpdateCurrentStatusTag(E_AITag::UNAWARE);

	RunBT();
}

void AAI_BaseController::RunBT()
{
	//Initializze BT
	if (ensureMsgf(MainBehaviorTree, TEXT("Behavior Tree is nullptr! Please assign a BehaviorTree in your AI controller")))
	{
		RunBehaviorTree(MainBehaviorTree);
		BrainTree = Cast<UBehaviorTreeComponent>(GetBrainComponent());
	}
}

/******* TEAM ID ******/

ETeamAttitude::Type AAI_BaseController::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (const APawn* OtherPawn = Cast<APawn>(&Other))
	{
		if (const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController()))
		{
			FGenericTeamId OtherTeamID = TeamAgent->GetGenericTeamId();
			if (OtherTeamID == 255)
			{
				return ETeamAttitude::Neutral;
			}
			else if (OtherTeamID == GetGenericTeamId())
			{
				return ETeamAttitude::Friendly;
			}
			else
			{
				return ETeamAttitude::Hostile;
			}
		}
		else
		{
			return ETeamAttitude::Hostile;
		}
	}

	return ETeamAttitude::Hostile;
}

void AAI_BaseController::UpdateCurrentStatusTag(E_AITag NewTag)
{
	switch (NewTag)
	{
		case E_AITag::UNAWARE:
		{
			CurrentStatusTag = UGameplayTagsManager::Get().RequestGameplayTag(TEXT("AIStatus.Unaware"));

			break;
		}

		case E_AITag::SUSPICIOUS:
		{
			CurrentStatusTag = UGameplayTagsManager::Get().RequestGameplayTag(TEXT("AIStatus.Suspicious"));

			break;
		}

		case E_AITag::ALERTED:
		{
			CurrentStatusTag = UGameplayTagsManager::Get().RequestGameplayTag(TEXT("AIStatus.Alerted"));

			break;
		}

		case E_AITag::HUNTING:
		{
			CurrentStatusTag = UGameplayTagsManager::Get().RequestGameplayTag(TEXT("AIStatus.Hunting"));

			break;
		}

		default:
		{
			break;
		}
	}
}

bool AAI_BaseController::CheckCurrentStatusTag(E_AITag TagToCheck)
{
	switch (TagToCheck)
	{
		case E_AITag::UNAWARE:
		{
			return GetCurrentStatusTag().MatchesTagExact(UGameplayTagsManager::Get().RequestGameplayTag(TEXT("AIStatus.Unaware")));
		}

		case E_AITag::SUSPICIOUS:
		{
			return GetCurrentStatusTag().MatchesTagExact(UGameplayTagsManager::Get().RequestGameplayTag(TEXT("AIStatus.Suspicious")));
		}

		case E_AITag::ALERTED:
		{
			return GetCurrentStatusTag().MatchesTagExact(UGameplayTagsManager::Get().RequestGameplayTag(TEXT("AIStatus.Alerted")));
		}

		case E_AITag::HUNTING:
		{
			return GetCurrentStatusTag().MatchesTagExact(UGameplayTagsManager::Get().RequestGameplayTag(TEXT("AIStatus.Hunting")));
		}

		default:
		{
			return false;;
		}
	}
}

void AAI_BaseController::SetupSightInfo()
{
	if (this == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Controller == nullptr"));
		return;
	}

	FAISenseID Id = UAISense::GetSenseID(UAISense_Sight::StaticClass());
	if (!Id.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Wrong Sense ID"));
		return;
	}

	auto Perception = GetAIPerceptionComponent();
	if (Perception == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Perception == nullptr"));
		return;
	}

	auto Config = Perception->GetSenseConfig(Id);
	if (Config == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Config == nullptr"));
		return;
	}

	auto ConfigSight = Cast<UAISenseConfig_Sight>(Config);

	ConfigSight->SightRadius = GetNPCRef()->AIInfo_DataAsset->SightRadius;
	ConfigSight->LoseSightRadius = GetNPCRef()->AIInfo_DataAsset->LoseSightRadius;
	ConfigSight->PeripheralVisionAngleDegrees = GetNPCRef()->AIInfo_DataAsset->PeripheralVisionAngleDegrees;

	Perception->RequestStimuliListenerUpdate();
}

void AAI_BaseController::SetupHearingInfo()
{
	if (this == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Controller == nullptr"));
		return;
	}

	FAISenseID Id = UAISense::GetSenseID(UAISense_Hearing::StaticClass());
	if (!Id.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Wrong Sense ID"));
		return;
	}

	auto Perception = GetAIPerceptionComponent();
	if (Perception == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Perception == nullptr"));
		return;
	}

	auto Config = Perception->GetSenseConfig(Id);
	if (Config == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Config == nullptr"));
		return;
	}

	auto ConfigSight = Cast<UAISenseConfig_Hearing>(Config);

	ConfigSight->HearingRange = GetNPCRef()->AIInfo_DataAsset->RunHearingRange;

	Perception->RequestStimuliListenerUpdate();
}

E_AISightCone AAI_BaseController::GetTypeOfSightCone(AActor* UpdatedActor)
{
	if (IsValid(UpdatedActor))
	{
		float CurrentDistance = (NPCRef->GetActorLocation() - UpdatedActor->GetActorLocation()).Size();
		float DotProduct = FVector::DotProduct(NPCRef->GetActorForwardVector(), (UpdatedActor->GetActorLocation() - NPCRef->GetActorLocation()).GetSafeNormal());
		float AngleTowardsTarget = UKismetMathLibrary::DegAcos(DotProduct);

		if (DotProduct >= 0)
		{
			if (AngleTowardsTarget <= NPCRef->AIInfo_DataAsset->SightPeripheralHalfAngleDegree_Narrow)
			{
				return E_AISightCone::NARROW;
			}
			else if (AngleTowardsTarget <= NPCRef->AIInfo_DataAsset->SightPeripheralHalfAngleDegree_Wide)
			{
				if (CurrentDistance < NPCRef->AIInfo_DataAsset->SightRadius_Wide)
				{
					return E_AISightCone::WIDE;
				}
				else
				{
					return E_AISightCone::NOTSEEN;
				}
			}
			else if (AngleTowardsTarget <= NPCRef->AIInfo_DataAsset->SightPeripheralHalfAngleDegree_Peripheral)
			{
				if (CurrentDistance < NPCRef->AIInfo_DataAsset->SightRadius_Peripheral)
				{
					return E_AISightCone::PERIPHERAL;
				}
				else
				{
					return E_AISightCone::NOTSEEN;
				}
			}
			else
			{
				return E_AISightCone::NOTSEEN;
			}
		}
		else
		{
			if (CurrentDistance <= NPCRef->AIInfo_DataAsset->SightRadius_Backward)
			{
				return E_AISightCone::BACKWARD;
			}
			else
			{
				return E_AISightCone::NOTSEEN;
			}
		}
	}

	return E_AISightCone::NOTSEEN;
}

void AAI_BaseController::ActorPerceivedUpdate(AActor* UpdatedActor, FAIStimulus Stimulus)
{
	if (!BrainComponent || !BrainComponent->IsRunning() || !IsValid(UpdatedActor))
	{
		return;
	}

	E_AISense CurrentSenseUsed = E_AISense::NONE;

	if (UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), Stimulus) == UAISense_Sight::StaticClass())
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			E_AISightCone CurrentTypeOfCone = GetTypeOfSightCone(UpdatedActor);

			if (CurrentTypeOfCone != E_AISightCone::NOTSEEN)
			{
				if (AwarenessComponent->GetCanUpdateAwareness())
				{
					AwarenessComponent->UpdateAwarenessFromSense(E_AISense::SIGHT, false, CurrentTypeOfCone);
				}
				else if (AlertComponent->GetCanUpdateAlert())
				{
					AlertComponent->UpdateAlertFromSense(E_AISense::SIGHT, CurrentTypeOfCone);
				}
			}
			else
			{
				return;
			}
		}

		CurrentSenseUsed = E_AISense::SIGHT;
	}
	else if (UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), Stimulus) == UAISense_Hearing::StaticClass())
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			if (Stimulus.Tag == "Run" || Stimulus.Tag == "Whistle" ||
				(Stimulus.Tag == "Walk" && ((NPCRef->GetActorLocation() - Stimulus.StimulusLocation).Size() <= GetNPCRef()->AIInfo_DataAsset->WalkHearingRange)))
			{
				if (AwarenessComponent->GetCanUpdateAwareness())
				{
					AwarenessComponent->UpdateAwarenessFromSense(E_AISense::HEARING, Stimulus.Tag == "Whistle" ? true : false);
				}
			}
			else
			{
				return;
			}
		}

		CurrentSenseUsed = E_AISense::HEARING;
	}
	else if (UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), Stimulus) == UAISense_Touch::StaticClass())
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			if (AwarenessComponent->GetCanUpdateAwareness())
			{
				AwarenessComponent->UpdateAwarenessFromSense(E_AISense::TOUCH);
				AlertComponent->UpdateAlertFromSense(E_AISense::TOUCH);
			}
			else if (AlertComponent->GetCanUpdateAlert())
			{
				AlertComponent->UpdateAlertFromSense(E_AISense::TOUCH);
			}
		}

		CurrentSenseUsed = E_AISense::TOUCH;
	}

	if (!CheckCurrentStatusTag(E_AITag::UNAWARE))
	{
		switch (CurrentSenseUsed)
		{
			case E_AISense::HEARING:
			{
				if (Stimulus.WasSuccessfullySensed())
				{
					if (!IsDisturbanceCooldownActive)
					{
						GetBlackboardComponent()->SetValueAsVector(KeyNameDisturbanceLocation, Stimulus.StimulusLocation);
						
						IsDisturbanceCooldownActive = true;
						GetWorld()->GetTimerManager().SetTimer(DisturbanceCooldownTimerHandle, this, &AAI_BaseController::OnDisturbanceCooldownFinished, GetNPCRef()->AIInfo_DataAsset->DisturbanceCooldown, false);
					}
				}

				break;
			}
			case E_AISense::SIGHT:
			{
				GetBlackboardComponent()->SetValueAsVector(KeyNameDisturbanceLocation, Stimulus.StimulusLocation);

				if (CheckCurrentStatusTag(E_AITag::ALERTED) || CheckCurrentStatusTag(E_AITag::HUNTING))
				{
					if (Stimulus.WasSuccessfullySensed())
					{
						OnLoseSightTimerFinished();
						GetBlackboardComponent()->SetValueAsBool(KeyNameIsTargetOnSight, true);
						GetBlackboardComponent()->SetValueAsObject(KeyNameTarget, UpdatedActor);
						UpdateCurrentStatusTag(E_AITag::ALERTED);
					}
					else
					{
						HasLoseSight = true;
						GetWorld()->GetTimerManager().SetTimer(LoseSightOnAlertedTimerHandle, this, &AAI_BaseController::OnLoseSightTimerFinished, GetNPCRef()->AIInfo_DataAsset->LoseSightTimer, false);
					}
				}
				else
				{
					GetBlackboardComponent()->SetValueAsBool(KeyNameIsTargetOnSight, Stimulus.WasSuccessfullySensed());
				}

				break;
			}
			case E_AISense::TOUCH:
			{
				if (Stimulus.WasSuccessfullySensed())
				{
					GetBlackboardComponent()->SetValueAsVector(KeyNameDisturbanceLocation, Stimulus.StimulusLocation);
				}

				GetBlackboardComponent()->SetValueAsBool(KeyNameHasTouchedTarget, Stimulus.WasSuccessfullySensed());

				break;
			}
		}
	}
}

void AAI_BaseController::OnDisturbanceCooldownFinished()
{
	GetWorld()->GetTimerManager().ClearTimer(DisturbanceCooldownTimerHandle);
	IsDisturbanceCooldownActive = false;
}

void AAI_BaseController::OnLoseSightTimerFinished()
{
	if (HasLoseSight)
	{
		GetBlackboardComponent()->SetValueAsBool(KeyNameIsTargetOnSight, false);
		UpdateCurrentStatusTag(E_AITag::HUNTING);
	}

	GetWorld()->GetTimerManager().ClearTimer(LoseSightOnAlertedTimerHandle);
	HasLoseSight = false;
}

void AAI_BaseController::ClearDisturbanceKey()
{
	GetBlackboardComponent()->ClearValue(KeyNameDisturbanceLocation);
}

void AAI_BaseController::ClearTargetKey()
{
	GetBlackboardComponent()->ClearValue(KeyNameTarget);
}

// DIRECTOR

void AAI_BaseController::SetupAIDirector()
{
	AIDirectorRef = Cast<AAI_Director>(UGameplayStatics::GetActorOfClass(GetWorld(), AAI_Director::StaticClass()));

	if (IsValid(AIDirectorRef))
	{
		AIDirectorRef->AddNPCToDirectorList(NPCRef);
	}
}

void AAI_BaseController::LinkToNPC(AAI_BaseNPC* Caller)
{
	if (IsValid(Caller) && Caller != LinkedNPC)
	{
		RemoveLinkToNPC();
		LinkedNPC = Caller;
	}
}

void AAI_BaseController::RemoveLinkToNPC()
{
	if (IsValid(LinkedNPC))
	{
		GetAIDirector()->RemoveLinkedNPC(LinkedNPC, GetNPCRef());

		LinkedNPC = nullptr;
	}
}

void AAI_BaseController::SetSuspiciousAsLinkedNPC()
{
	if (IsValid(LinkedNPC))
	{
		GetAwarenessComponent()->UpdateAwarenessValue(LinkedNPC->GetAICRef()->GetAwarenessComponent()->GetCurrentAwarenessValue());
		GetBlackboardComponent()->SetValueAsVector(KeyNameDisturbanceLocation, LinkedNPC->GetAICRef()->GetBlackboardComponent()->GetValueAsVector(KeyNameDisturbanceLocation));
	}
}

void AAI_BaseController::SetHuntingAsLinkedNPC()
{
	if (IsValid(LinkedNPC))
	{
		GetAwarenessComponent()->UpdateAwarenessValue(LinkedNPC->GetAICRef()->GetAwarenessComponent()->GetCurrentAwarenessValue());
		GetAlertComponent()->UpdateAlertValue(LinkedNPC->GetAICRef()->GetAlertComponent()->GetCurrentAlertValue(), true);
		GetBlackboardComponent()->SetValueAsVector(KeyNameDisturbanceLocation, LinkedNPC->GetAICRef()->GetBlackboardComponent()->GetValueAsVector(KeyNameDisturbanceLocation));
	}
}