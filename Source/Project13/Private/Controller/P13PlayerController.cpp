// Copyright © 2024, IKinder

#include "Controller/P13PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "NiagaraFunctionLibrary.h"
#include "Project13.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Character/P13CharacterBase.h"
#include "Components/DecalComponent.h"
#include "Game/P13GameMode.h"
#include "Game/P13HeadsUpDisplay.h"
#include "Game/P13PlayerState.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Pawn.h"
#include "Intearface/P13InputInterface.h"
#include "Kismet/GameplayStatics.h"

AP13PlayerController::AP13PlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	bReplicates = true;
}

void AP13PlayerController::BeginPlay()
{
	Super::BeginPlay();
	AddDefaultMappingContext();
	SpawnCursorDecal();
	SetNewInputMode();
}

void AP13PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	auto* InputComp = Cast<UEnhancedInputComponent>(InputComponent);
	if (!InputComp)
	{
		return;
	}
	InputComp->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::MoveInput);
	InputComp->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::LookInput);

	InputComp->BindAction(SetDestinationAction, ETriggerEvent::Started, this, &ThisClass::OnInputStarted);
	InputComp->BindAction(SetDestinationAction, ETriggerEvent::Triggered, this, &ThisClass::OnSetDestinationTriggered);
	InputComp->BindAction(SetDestinationAction, ETriggerEvent::Completed, this, &ThisClass::OnSetDestinationReleased);
	InputComp->BindAction(SetDestinationAction, ETriggerEvent::Canceled, this, &ThisClass::OnSetDestinationReleased);

	InputComp->BindAction(SprintAction, ETriggerEvent::Started, this, &ThisClass::SprintInput, true);
	InputComp->BindAction(SprintAction, ETriggerEvent::Completed, this, &ThisClass::SprintInput, false);

	InputComp->BindAction(AimAction, ETriggerEvent::Started, this, &ThisClass::AimInput);

	InputComp->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &ThisClass::ZoomInput);

	InputComp->BindAction(FireAction, ETriggerEvent::Started, this, &ThisClass::FireInput, true);
	InputComp->BindAction(FireAction, ETriggerEvent::Completed, this, &ThisClass::FireInput, false);

	InputComp->BindAction(ReloadAction, ETriggerEvent::Started, this, &ThisClass::ReloadInput);

	InputComp->BindAction(NextWeaponAction, ETriggerEvent::Started, this, &ThisClass::SwitchWeaponInput, true);
	InputComp->BindAction(PreviousWeaponAction, ETriggerEvent::Started, this, &ThisClass::SwitchWeaponInput, false);

	InputComp->BindAction(DropAction, ETriggerEvent::Started, this, &ThisClass::DropInput, true);
}

void AP13PlayerController::OnPossess(APawn* InPawn)
{
	if (!InPawn)
	{
		return;
	}

	Super::OnPossess(InPawn);
}

void AP13PlayerController::OnUnPossess()
{
	Super::OnUnPossess();

	if (HeadsUpDisplay)
	{
		HeadsUpDisplay->ClearInGame();
	}

	if (TryRespawnOnUnPossess())
	{
		return;
	}

	/* After the final UnPossess we need to show End Game widget. */
	SetNewInputMode(false);
	CachedCursorDecal->SetWorldTransform(FTransform::Identity);

	if (HeadsUpDisplay)
	{
		HeadsUpDisplay->ShowEndGame(false, this);
	}
}

void AP13PlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
}

void AP13PlayerController::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (IsLocalController())
	{
		FindPointUnderCursor();
		UpdateCursorDecalPosition();
	}
}

void AP13PlayerController::ShowHeadsUpDisplay()
{
	HeadsUpDisplay = GetHUD<AP13HeadsUpDisplay>();
	if (HeadsUpDisplay)
	{
		HeadsUpDisplay->ShowInGame(this);
	}
}

void AP13PlayerController::OnGameWon()
{
	check(HeadsUpDisplay)

	HeadsUpDisplay->ShowEndGame(true, this);

	SetNewInputMode(false);
	Pause();
}

void AP13PlayerController::Multicast_UpdatePlayerColor_Implementation(const FLinearColor FoundColor)
{
	PawnTrueColor = FoundColor;

	/* Send found color to the controlled pawn. */
	if (AP13CharacterBase* ControlledCharacter = GetPawn<AP13CharacterBase>())
	{
		ControlledCharacter->UpdateDynamicMeshMaterials(PawnTrueColor);
	}
}

void AP13PlayerController::OnInputStarted()
{
	StopMovement();
}

void AP13PlayerController::OnSetDestinationTriggered()
{
	if (!GetCanControlledCharacterMove())
	{
		return;
	}

	/* We flag that the input is being pressed. */
	FollowTime += GetWorld()->GetDeltaSeconds();

	/* If we hit a surface, cache the location. */
	if (HitUnderCursor.bBlockingHit)
	{
		CachedDestination = HitUnderCursor.Location;
	}

	/* Move towards mouse pointer. */
	ACharacter* OurChar = GetCharacter();
	if (!OurChar)
	{
		return;
	}
	const FVector WorldDirection = (CachedDestination - OurChar->GetActorLocation()).GetSafeNormal();
	OurChar->AddMovementInput(WorldDirection, 1.0, false);
}

void AP13PlayerController::OnSetDestinationReleased()
{
	if (!GetCanControlledCharacterMove())
	{
		return;
	}

	/* If it was a short press. */
	if (FollowTime <= ShortPressThreshold)
	{
		/* We move there and spawn some particles. */
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, CursorClickFX, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}
	FollowTime = 0.f;
}

void AP13PlayerController::MoveInput(const FInputActionValue& Value)
{
	StopMovement();

	if (IP13InputInterface* InputInterface = Cast<IP13InputInterface>(GetPawn()))
	{
		InputInterface->MoveInput(Value.Get<FVector2D>());
	}
}

void AP13PlayerController::LookInput(const FInputActionValue& Value)
{}

void AP13PlayerController::SprintInput(const bool bStart)
{
	if (IP13InputInterface* InputInterface = Cast<IP13InputInterface>(GetPawn()))
	{
		InputInterface->SprintInput(bStart);
	}
}

void AP13PlayerController::AimInput()
{
	if (IP13InputInterface* InputInterface = Cast<IP13InputInterface>(GetPawn()))
	{
		InputInterface->AimInput();
	}
}

void AP13PlayerController::ZoomInput(const FInputActionValue& Value)
{
	if (IP13InputInterface* InputInterface = Cast<IP13InputInterface>(GetPawn()))
	{
		InputInterface->ZoomInput(Value.Get<float>());
	}
}

void AP13PlayerController::FireInput(const bool bStart)
{
	if (IP13InputInterface* InputInterface = Cast<IP13InputInterface>(GetPawn()))
	{
		InputInterface->FireInput(bStart);
	}
}

void AP13PlayerController::ReloadInput()
{
	if (IP13InputInterface* InputInterface = Cast<IP13InputInterface>(GetPawn()))
	{
		InputInterface->ReloadInput();
	}
}

void AP13PlayerController::SwitchWeaponInput(const bool bNext)
{
	if (IP13InputInterface* InputInterface = Cast<IP13InputInterface>(GetPawn()))
	{
		InputInterface->SwitchWeaponInput(bNext);
	}
}

void AP13PlayerController::DropInput(const bool bTakeNext)
{
	if (IP13InputInterface* InputInterface = Cast<IP13InputInterface>(GetPawn()))
	{
		InputInterface->DropInput(bTakeNext);
	}
}

void AP13PlayerController::AddDefaultMappingContext()
{
	auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (!Subsystem)
	{
		return;
	}
	Subsystem->AddMappingContext(DefaultContext, 0);
}

void AP13PlayerController::RotateTowardCursorSmoothly(const FVector Direction)
{
	if (IP13InputInterface* InputInterface = Cast<IP13InputInterface>(GetPawn()))
	{
		InputInterface->RotateTowardMovement(Direction);
	}
}

void AP13PlayerController::FindPointUnderCursor()
{
	/* We look for the location in the world where the player has pressed the input. */
	if (GetHitResultUnderCursor(ECC_CURSOR, true, HitUnderCursor))
	{
		OnHitUnderCursorChanged.Broadcast(this, HitUnderCursor);
	}
}

void AP13PlayerController::SpawnCursorDecal()
{
	if (!CursorMaterial || !IsLocalController())
	{
		return;
	}
	CachedCursorDecal = UGameplayStatics::SpawnDecalAtLocation(this, CursorMaterial, CursorSize, FVector::ZeroVector);
}

void AP13PlayerController::UpdateCursorDecalPosition() const
{
	if (!IsLocalController() || !GetCanControlledCharacterMove() || !CachedCursorDecal.IsValid())
	{
		return;
	}

	const FVector CursorLocation = HitUnderCursor.bBlockingHit ? HitUnderCursor.Location : FVector::ZeroVector;
	const FRotator CursorRotation = HitUnderCursor.bBlockingHit ? HitUnderCursor.ImpactNormal.Rotation() : FRotator::ZeroRotator;

	CachedCursorDecal->SetWorldLocation(CursorLocation);
	CachedCursorDecal->SetWorldRotation(CursorRotation);
}

bool AP13PlayerController::GetCanControlledCharacterMove() const
{
	const ACharacter* ControlledCharacter = GetCharacter();
	if (!ControlledCharacter)
	{
		return false;
	}
	const UCharacterMovementComponent* CharacterMovement = ControlledCharacter->GetCharacterMovement();
	if (!CharacterMovement)
	{
		return false;
	}
	if (CharacterMovement->MovementMode == MOVE_None)
	{
		return false;
	}
	return true;
}

void AP13PlayerController::SetNewInputMode(const bool bGame)
{
	if (bGame)
	{
		FInputModeGameOnly GIMode;
		GIMode.SetConsumeCaptureMouseDown(false);

		SetInputMode(GIMode);
		bShowMouseCursor = false;
		return;
	}

	/* Means UI. */
	FInputModeUIOnly UIMode;
	UIMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen);

	SetInputMode(UIMode);
	bShowMouseCursor = true;
}

bool AP13PlayerController::TryRespawnOnUnPossess()
{
	AP13GameMode* GameMode = GetWorld()->GetAuthGameMode<AP13GameMode>();
	if (!GameMode)
	{
		return false;
	}

	const AP13PlayerState* DeadPlayerState = GetPlayerState<AP13PlayerState>();
	if (!DeadPlayerState)
	{
		return false;
	}

	if (!DeadPlayerState->GetPlayerCanRespawn())
	{
		return false;
	}

	GameMode->RespawnPlayer(this);
	return true;
}
