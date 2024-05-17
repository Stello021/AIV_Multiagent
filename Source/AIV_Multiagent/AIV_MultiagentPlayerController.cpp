// Copyright Epic Games, Inc. All Rights Reserved.

#include "AIV_MultiagentPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "AIV_MultiagentCharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionSystem.h"
#include "Perception/AISense_Sight.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AAIV_MultiagentPlayerController::AAIV_MultiagentPlayerController()
{
	DefaultTargetArmLength = -1.f;
	CameraZoomDifference = 250.f;
	CameraPositionInt = 3;
	MaxCameraPositionInt = 5;
}

void AAIV_MultiagentPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(CustomMappingContext, 0);
	}

	PlayerRef = Cast<AAIV_MultiagentCharacter>(GetPawn());
	if (IsValid(PlayerRef))
	{
		PlayerRef->PlayerController = this;

		DefaultTargetArmLength = PlayerRef->GetCameraBoom()->TargetArmLength;
		PlayerRef->GetCameraBoom()->TargetArmLength = DefaultTargetArmLength + (CameraZoomDifference * CameraPositionInt);

		PlayerRef->GetCharacterMovement()->MaxWalkSpeed = 230.f;
	}
}

void AAIV_MultiagentPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(PlayerRef) && DefaultTargetArmLength > 0.f)
	{
		PlayerRef->GetCameraBoom()->TargetArmLength = FMath::FInterpTo(PlayerRef->GetCameraBoom()->TargetArmLength, (DefaultTargetArmLength + (CameraZoomDifference * CameraPositionInt)), DeltaTime, 2.f);
	}
}

void AAIV_MultiagentPlayerController::SetIsHiding(bool IsNowHiding)
{
	IsHiding = IsNowHiding;

	if (IsHiding)
	{
		UAIPerceptionSystem::GetCurrent(GetWorld())->UnregisterSource(*PlayerRef, UAISense_Sight::StaticClass());
	}
	else
	{
		UAIPerceptionSystem::RegisterPerceptionStimuliSource(GetWorld(), UAISense_Sight::StaticClass(), PlayerRef);
	}
}

void AAIV_MultiagentPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAIV_MultiagentPlayerController::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AAIV_MultiagentPlayerController::Look);
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &AAIV_MultiagentPlayerController::Zoom);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AAIV_MultiagentPlayerController::StartSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AAIV_MultiagentPlayerController::StopSprint);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AAIV_MultiagentPlayerController::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (IsValid(PlayerRef))
	{		
		PlayerRef->AddMovementInput(PlayerRef->GetCameraBoom()->GetForwardVector(), MovementVector.Y * 2.f);
		PlayerRef->AddMovementInput(PlayerRef->GetCameraBoom()->GetRightVector(), MovementVector.X);
	}
}

void AAIV_MultiagentPlayerController::Look(const FInputActionValue& Value)
{
	const float LookAxisVector = Value.Get<float>();

	if (IsValid(PlayerRef))
	{
		if (IsInputKeyDown(EKeys::RightMouseButton))
		{
			FRotator Rotator = FRotator(0.f, LookAxisVector, 0.f);
			PlayerRef->GetCameraBoom()->AddWorldRotation(Rotator);
		}
	}
}

void AAIV_MultiagentPlayerController::Zoom(const FInputActionValue& Value)
{
	const float ZoomAxisVector = Value.Get<float>();

	if (ZoomAxisVector > 0)
	{
		CameraPositionInt++;
	}
	else if (ZoomAxisVector < 0)
	{
		CameraPositionInt--;
	}
	CameraPositionInt = FMath::Clamp(CameraPositionInt, 0, MaxCameraPositionInt);

}

void AAIV_MultiagentPlayerController::StartSprint(const FInputActionValue& Value)
{
	const bool SprintValue = Value.Get<bool>();

	if (!IsSprinting)
	{
		PlayerRef->GetCharacterMovement()->MaxWalkSpeed = 500.f;
		IsSprinting = true;
	}
}

void AAIV_MultiagentPlayerController::StopSprint(const FInputActionValue& Value)
{
	if (IsSprinting)
	{
		PlayerRef->GetCharacterMovement()->MaxWalkSpeed = 230.f;
		IsSprinting = false;
	}
}
