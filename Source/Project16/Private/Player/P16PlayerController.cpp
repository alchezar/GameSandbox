// Copyright © 2025, Ivan Kinder

#include "Player/P16PlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayTagContainer.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Project16.h"
#include "AbilitySystem/P16AbilitySystemComponent.h"
#include "Component/P16InputComponent.h"
#include "Components/SplineComponent.h"
#include "Interface/P16InterfaceEnemy.h"
#include "Root/Public/Singleton/GSGameplayTagsSingleton.h"

AP16PlayerController::AP16PlayerController()
{
	SetReplicates(true);

	Spline = CreateDefaultSubobject<USplineComponent>("PathSplineComponent");
}

void AP16PlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ControlledPawn = InPawn;
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
	AutoRun();
}

void AP16PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(InputComponent);
	check(Input)

	Input->BindAction(MoveAction.Get(), ETriggerEvent::Triggered, this, &ThisClass::MoveInputCallback);
	Input->BindAction(ShiftAction.Get(), ETriggerEvent::Started, this, &ThisClass::ShiftInputCallback, true);
	Input->BindAction(ShiftAction.Get(), ETriggerEvent::Completed, this, &ThisClass::ShiftInputCallback, false);

	// Using custom input component for binding all input actions to their tags.
	if (UP16InputComponent* InputComp = Cast<UP16InputComponent>(InputComponent))
	{
		InputComp->BindAbilityActions(InputConfig, this,
			&ThisClass::AbilityInputTagPressed,
			&ThisClass::AbilityInputTagReleased,
			&ThisClass::AbilityInputTagHeld);
	}
}

void AP16PlayerController::MoveInputCallback(const FInputActionValue& InputValue)
{
	const FVector2D InputVector      = InputValue.Get<FVector2D>();
	const FRotator  YawRotation      = {0.f, GetControlRotation().Yaw, 0.f};
	const FVector   ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector   RightDirection   = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (ControlledPawn)
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputVector.X);
	}
}

void AP16PlayerController::ShiftInputCallback(const FInputActionValue& InputValue, const bool bDown)
{
	bShiftKeyDown = bDown;
}

void AP16PlayerController::CursorTrace()
{
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

void AP16PlayerController::AutoRun()
{
	EARLY_RETURN_IF(!bAutoRunning || !ControlledPawn)

	const float SquaredDistance     = FVector::DistSquared(ControlledPawn->GetActorLocation(), CachedDestination);
	const float SquaredAcceptRadius = AutoRunAcceptRadius * AutoRunAcceptRadius;
	if (SquaredDistance < SquaredAcceptRadius)
	{
		bAutoRunning = false;
		return;
	}

	const FVector Direction = Spline->FindDirectionClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
	ControlledPawn->AddMovementInput(Direction);
}

void AP16PlayerController::AbilityInputTagPressed(const FGameplayTag InputTag)
{
	if (GetIsLMB(InputTag))
	{
		bTargeting   = LastTickEnemy != nullptr;
		bAutoRunning = false;
		FollowTime   = 0.f;
	}
}

void AP16PlayerController::AbilityInputTagReleased(const FGameplayTag InputTag)
{
	// Activate ability.
	if (bTargeting || !GetIsLMB(InputTag))
	{
		EARLY_RETURN_IF(!GetAbilitySystemComponent())
		AbilitySystemComponent->AbilityInputTagReleased(InputTag);
		return;
	}

	// Move towards click behavior.
	EARLY_RETURN_IF(FollowTime > ClickThreshold || !ControlledPawn)
	UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination);
	EARLY_RETURN_IF(!NavPath)

	Spline->ClearSplinePoints();
	for (const FVector& PathPoint : NavPath->PathPoints)
	{
		Spline->AddSplinePoint(PathPoint, ESplineCoordinateSpace::World, false);
		CachedDestination = PathPoint;
	}
	bAutoRunning = true;
}

void AP16PlayerController::AbilityInputTagHeld(const FGameplayTag InputTag)
{
	// Activate ability.
	if (!GetIsLMB(InputTag) || bTargeting || bShiftKeyDown)
	{
		EARLY_RETURN_IF(!GetAbilitySystemComponent())
		AbilitySystemComponent->AbilityInputTagHeld(InputTag);
		return;
	}

	// Follow mouse behavior.
	FollowTime += GetWorld()->GetDeltaSeconds();
	EARLY_RETURN_IF(!CursorHit.bBlockingHit)
	CachedDestination = CursorHit.ImpactPoint;

	EARLY_RETURN_IF(!ControlledPawn)
	const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal2D();
	ControlledPawn->AddMovementInput(WorldDirection);
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

UP16AbilitySystemComponent* AP16PlayerController::GetAbilitySystemComponent()
{
	if (!AbilitySystemComponent)
	{
		AbilitySystemComponent = Cast<UP16AbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()));
	}

	return AbilitySystemComponent;
}

bool AP16PlayerController::GetIsLMB(const FGameplayTag InputTag) const
{
	return InputTag.MatchesTagExact(FGSGameplayTagsSingleton::Get().P16Tags.Input_MouseButtonLeft);
}
