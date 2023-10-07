// Copyright (C) 2023, IKinder

#include "P12/Public/Player/Controller/P12PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "P12/Public/Player/P12BaseCharacter.h"

class UEnhancedInputLocalPlayerSubsystem;

AP12PlayerController::AP12PlayerController()
{
	
}

void AP12PlayerController::BeginPlay()
{
	Super::BeginPlay();
	SubsystemDefaultMappingContext();
}

/*---------------------------------------------------------------------------*/

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
	InputComp->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::LookInput);
	InputComp->BindAction(JumpAction, ETriggerEvent::Started, this, &ThisClass::JumpInput);
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

void AP12PlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	CachedBaseCharacter = Cast<AP12BaseCharacter>(InPawn);
}
