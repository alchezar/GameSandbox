// Copyright (C) 2023, IKinder

#include "P12/Public/Player/Controller/P12PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "P12/Public/Component/Actor/P12AttributeComponent.h"
#include "P12/Public/Game/P12HUD.h"
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
	InputComp->BindAction(ReloadAction, ETriggerEvent::Started, this, &ThisClass::ReloadInput);
	InputComp->BindAction(NextItemAction, ETriggerEvent::Started, this, &ThisClass::EquipItemInput, true);
	InputComp->BindAction(PreviousItemAction, ETriggerEvent::Started, this, &ThisClass::EquipItemInput, false);
	InputComp->BindAction(EquipProjectileAction, ETriggerEvent::Started, this, &ThisClass::EquipThrowableInput);
	InputComp->BindAction(PrimaryMeleeAction, ETriggerEvent::Started, this, &ThisClass::PrimaryMeleeInput);
	InputComp->BindAction(SecondaryMeleeAction, ETriggerEvent::Started, this, &ThisClass::SecondaryMeleeInput);
	InputComp->BindAction(MainMenuAction, ETriggerEvent::Started, this, &ThisClass::ShowMainMenu);
	InputComp->BindAction(InteractAction, ETriggerEvent::Started, this, &ThisClass::InteractInput);
}

bool AP12PlayerController::GetCharacterCanProcessInput() const
{
	return !CachedBaseCharacter.IsNull()
		&& CachedBaseCharacter->GetAttributeComponent()
		&& CachedBaseCharacter->GetAttributeComponent()->GetIsAlive();
}

void AP12PlayerController::MoveInput(const FInputActionValue& Value) 
{
	if (!GetCharacterCanProcessInput())
	{
		return;
	}
	CachedBaseCharacter->MoveInput(Value);
}

void AP12PlayerController::LookInput(const FInputActionValue& Value) 
{
	if (!GetCharacterCanProcessInput())
	{
		return;
	}
	CachedBaseCharacter->LookInput(Value);
}

void AP12PlayerController::JumpInput() 
{
	if (!GetCharacterCanProcessInput())
	{
		return;
	}
	CachedBaseCharacter->JumpInput();
}

void AP12PlayerController::MantleInput()
{
	if (!GetCharacterCanProcessInput())
	{
		return;
	}
	CachedBaseCharacter->MantleInput();
}

void AP12PlayerController::CrouchInput()
{
	if (!GetCharacterCanProcessInput())
	{
		return;
	}
	CachedBaseCharacter->CrouchInput();
}

void AP12PlayerController::RunInput(const bool bRun)
{
	if (!GetCharacterCanProcessInput())
	{
		return;
	}
	CachedBaseCharacter->RunInput(bRun);
}

void AP12PlayerController::LadderJumpInput() 
{
	if (!GetCharacterCanProcessInput())
	{
		return;
	}
	CachedBaseCharacter->LadderJumpInput();
}

void AP12PlayerController::LadderClimbInput(const FInputActionValue& Value) 
{
	if (!GetCharacterCanProcessInput())
	{
		return;
	}
	CachedBaseCharacter->LadderClimbInput(Value);
}

void AP12PlayerController::FireInput(const bool bStart)
{
	if (!GetCharacterCanProcessInput())
	{
		return;
	}
	CachedBaseCharacter->FireInput(bStart);
}

void AP12PlayerController::AimInput(const bool bStart)
{
	if (!GetCharacterCanProcessInput())
	{
		return;
	}
	CachedBaseCharacter->AimInput(bStart);
}

void AP12PlayerController::ReloadInput()
{
	if (!GetCharacterCanProcessInput())
	{
		return;
	}
	CachedBaseCharacter->ReloadInput();
}

void AP12PlayerController::EquipItemInput(const bool bNext)
{
	if (!GetCharacterCanProcessInput())
	{
		return;
	}
	CachedBaseCharacter->EquipItemInput(bNext);
}

void AP12PlayerController::EquipThrowableInput()
{
	if (!GetCharacterCanProcessInput())
	{
		return;
	}
	CachedBaseCharacter->EquipThrowableInput();
}

void AP12PlayerController::PrimaryMeleeInput() 
{
	if (!GetCharacterCanProcessInput())
	{
		return;
	}
	CachedBaseCharacter->PrimaryMeleeInput();
}

void AP12PlayerController::SecondaryMeleeInput() 
{
	if (!GetCharacterCanProcessInput())
	{
		return;
	}
	CachedBaseCharacter->SecondaryMeleeInput();
}

void AP12PlayerController::ShowMainMenu()
{
	AP12HUD* HUD = GetHUD<AP12HUD>();
	if (!HUD)
	{
		return;
	}
	HUD->ShowMainMenu(true);
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

void AP12PlayerController::ToggleMenuInputMode(const bool bMenu)
{
	SetShowMouseCursor(bMenu);

	FInputModeGameAndUI GameAndUIMode;
	GameAndUIMode.SetHideCursorDuringCapture(false);
	bMenu ? SetInputMode(GameAndUIMode) : SetInputMode(FInputModeGameOnly());
	bMenu ? GetPawn()->DisableInput(this) : GetPawn()->EnableInput(this);
	bMenu ? DisableInput(this) : EnableInput(this);

	if (GetNetMode() == NM_Standalone)
	{
		SetPause(bMenu);
	}
}

void AP12PlayerController::InteractInput()
{
	if (!GetCharacterCanProcessInput())
	{
		return;
	}
	CachedBaseCharacter->Interact();
}
