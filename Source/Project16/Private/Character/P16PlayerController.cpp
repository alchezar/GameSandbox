// Copyright © 2025, Ivan Kinder

#include "Character/P16PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Project16.h"
#include "Interface/P16InterfaceEnemy.h"

AP16PlayerController::AP16PlayerController()
{
	SetReplicates(true);
}

void AP16PlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void AP16PlayerController::BeginPlay()
{
	Super::BeginPlay();

	AddDefaultMappingContext();

	bShowMouseCursor   = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AP16PlayerController::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	CursorTrace();
}

void AP16PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(InputComponent);
	check(Input)

	Input->BindAction(MoveAction.Get(), ETriggerEvent::Triggered, this, &ThisClass::MoveInputCallback);
}

void AP16PlayerController::MoveInputCallback(const FInputActionValue& InputValue)
{
	const FVector2D InputVector      = InputValue.Get<FVector2D>();
	const FRotator  YawRotation      = {0.f, GetControlRotation().Yaw, 0.f};
	const FVector   ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector   RightDirection   = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputVector.X);
	}
}

void AP16PlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Camera, false, CursorHit);
	EARLY_RETURN_IF(!CursorHit.bBlockingHit)

	// Do nothing if LastTickEnemy == ThisTickEnemy, no matter they are nullptr or valid.
	// In other cases, we always want to toggle highlight between the last and this tick enemies.
	const TScriptInterface<IP16InterfaceEnemy> ThisTickEnemy = CursorHit.GetActor();
	if (LastTickEnemy != ThisTickEnemy)
	{
		if (LastTickEnemy)
		{
			LastTickEnemy->ToggleHighlight(false);
		}
		if (ThisTickEnemy)
		{
			ThisTickEnemy->ToggleHighlight(true);
		}
	}
	LastTickEnemy = ThisTickEnemy;
}

void AP16PlayerController::AddDefaultMappingContext() const
{
	check(MappingContext)

	const ULocalPlayer* LocalPlayer = GetLocalPlayer();
	EARLY_RETURN_IF(!LocalPlayer)
	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	EARLY_RETURN_IF(!Subsystem)

	Subsystem->AddMappingContext(MappingContext.Get(), 0);
}
