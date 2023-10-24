// Copyright (C) 2023, IKinder

#include "P12/Public/Player/Controller/P12PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "P12/Public/Player/P12BaseCharacter.h"
#include "P12/Public/Util/P12Library.h"

class UEnhancedInputLocalPlayerSubsystem;

AP12PlayerController::AP12PlayerController()
{
	
}

void AP12PlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	CachedBaseCharacter = Cast<AP12BaseCharacter>(InPawn);
}

void AP12PlayerController::BeginPlay()
{
	Super::BeginPlay();
	SubsystemDefaultMappingContext();
}

void AP12PlayerController::SubsystemDefaultMappingContext() const
{
	auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (!Subsystem)
	{
		return;
	}
	Subsystem->AddMappingContext(DefaultContext, 0);
}

void AP12PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	auto* InputComp = Cast<UEnhancedInputComponent>(InputComponent);
	if (!InputComp)
	{
		return;
	}
	InputComp->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::MoveInput);
	InputComp->BindAction(MoveAction, ETriggerEvent::Completed, this, &ThisClass::MoveInput);
	InputComp->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::LookInput);
	InputComp->BindAction(JumpAction, ETriggerEvent::Started, this, &ThisClass::JumpInput);
	InputComp->BindAction(CrouchAction, ETriggerEvent::Started, this, &ThisClass::CrouchInput);
	InputComp->BindAction(RunAction, ETriggerEvent::Started, this, &ThisClass::RunInput, true);
	InputComp->BindAction(RunAction, ETriggerEvent::Completed, this, &ThisClass::RunInput, false);
	InputComp->BindAction(MantleAction, ETriggerEvent::Started, this, &ThisClass::MantleInput);
	InputComp->BindAction(LadderJumpAction, ETriggerEvent::Started, this, &ThisClass::LadderJumpInput);
	InputComp->BindAction(LadderClimbAction, ETriggerEvent::Triggered, this, &ThisClass::LadderClimbInput);

	InputComp->BindAction(FireAction, ETriggerEvent::Started, this, &ThisClass::FireInput, true);
	InputComp->BindAction(FireAction, ETriggerEvent::Completed, this, &ThisClass::FireInput, false);
	InputComp->BindAction(AimAction, ETriggerEvent::Started, this, &ThisClass::AimInput, true);
	InputComp->BindAction(AimAction, ETriggerEvent::Completed, this, &ThisClass::AimInput, false);
}

void AP12PlayerController::MoveInput(const FInputActionValue& Value) 
{
	if (CachedBaseCharacter.IsNull())
	{
		return;
	}
	CachedBaseCharacter->MoveInput(Value);
}

void AP12PlayerController::LookInput(const FInputActionValue& Value) 
{
	if (CachedBaseCharacter.IsNull())
	{
		return;
	}
	CachedBaseCharacter->LookInput(Value);
}

void AP12PlayerController::JumpInput() 
{
	if (CachedBaseCharacter.IsNull())
	{
		return;
	}
	CachedBaseCharacter->JumpInput();
}

void AP12PlayerController::MantleInput()
{
	if (CachedBaseCharacter.IsNull())
	{
		return;
	}
	CachedBaseCharacter->MantleInput();
}

void AP12PlayerController::CrouchInput()
{
	if (CachedBaseCharacter.IsNull())
	{
		return;
	}
	CachedBaseCharacter->CrouchInput();
}

void AP12PlayerController::RunInput(const bool bRun)
{
	if (CachedBaseCharacter.IsNull())
	{
		return;
	}
	CachedBaseCharacter->RunInput(bRun);
}

void AP12PlayerController::LadderJumpInput() 
{
	if (CachedBaseCharacter.IsNull())
	{
		return;
	}
	CachedBaseCharacter->LadderJumpInput();
}

void AP12PlayerController::LadderClimbInput(const FInputActionValue& Value) 
{
	if (CachedBaseCharacter.IsNull())
	{
		return;
	}
	CachedBaseCharacter->LadderClimbInput(Value);
}

void AP12PlayerController::FireInput(const bool bStart)
{
	if (CachedBaseCharacter.IsNull())
	{
		return;
	}
	CachedBaseCharacter->FireInput(bStart);
}

void AP12PlayerController::AimInput(const bool bStart)
{
	if (CachedBaseCharacter.IsNull())
	{
		return;
	}
	CachedBaseCharacter->AimInput(bStart);
}

void AP12PlayerController::P12Debug_EnableAll()
{
	TArray<FString> ConsoleVariables = UP12Library::GetAllConsoleVariables();
	for(const FString& ConsoleVariable : ConsoleVariables)
	{
		ConsoleCommand( ConsoleVariable + " 1");
	}
}

void AP12PlayerController::P12Debug_DisableAll()
{
	TArray<FString> ConsoleVariables = UP12Library::GetAllConsoleVariables();
	for(const FString& ConsoleVariable : ConsoleVariables)
	{
		ConsoleCommand( ConsoleVariable + " 0");
	}
}
