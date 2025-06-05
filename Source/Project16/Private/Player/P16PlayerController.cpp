// Copyright © 2025, Ivan Kinder

#include "Player/P16PlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayTagContainer.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Project16.h"
#include "AbilitySystem/P16AbilitySystemComponent.h"
#include "Actor/P16MagicCircle.h"
#include "Component/P16InputComponent.h"
#include "Components/SplineComponent.h"
#include "Interface/P16HighlightInterface.h"
#include "Root/Public/Singleton/GSGameplayTagsSingleton.h"
#include "UI/Component/P16DamageTextComponent.h"

AP16PlayerController::AP16PlayerController()
{
	bReplicates = true;

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
	UpdateMagicCircle();
}

void AP16PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(InputComponent);
	check(Input);

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

void AP16PlayerController::Client_ShowDamageNumber_Implementation(const float InDamage, AActor* Target, const bool bBlockedHit, const bool bCriticalHit)
{
	EARLY_RETURN_IF(!Target || !DamageTextComponentClass || !IsLocalController())

	constexpr float MaxOffset    = 50.f;
	const FVector   RandomOffset = FVector {FMath::RandRange(-MaxOffset, MaxOffset), FMath::RandRange(-MaxOffset, MaxOffset), 0.f};

	UP16DamageTextComponent* DamageText = NewObject<UP16DamageTextComponent>(Target, DamageTextComponentClass);
	DamageText->RegisterComponent();
	DamageText->SetWorldLocation(Target->GetActorLocation() + RandomOffset);
	DamageText->SetDamageText(InDamage, bBlockedHit, bCriticalHit);
}

void AP16PlayerController::ToggleMagicCircle(const bool bOn, UMaterialInterface* DecalMaterial)
{
	if (MagicCircle)
	{
		MagicCircle->Destroy();
		MagicCircle = nullptr;
	}

	EARLY_RETURN_IF(!bOn || !MagicCircleClass)
	MagicCircle = GetWorld()->SpawnActor<AP16MagicCircle>(MagicCircleClass);

	EARLY_RETURN_IF(!MagicCircle || !DecalMaterial)
	MagicCircle->SetDecalMaterial(DecalMaterial);
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
	EARLY_RETURN_IF(!GetAbilitySystemComponent())
	if (GetAbilitySystemComponent()->HasMatchingGameplayTag(FGSGameplayTagsSingleton::Get().P16Tags.Player.Blocked.CursorTraceTag))
	{
		if (LastTickActor)
		{
			IP16HighlightInterface::Execute_ToggleHighlight(LastTickActor, false);
			LastTickActor = nullptr;
		}
		return;
	}

	GetHitResultUnderCursor(ECC_Camera, false, CursorHit);
	EARLY_RETURN_IF(!CursorHit.bBlockingHit)

	// Do nothing if LastTickEnemy == ThisTickEnemy, no matter they are nullptr or valid.
	// In other cases, we always want to toggle highlight between the last and this tick enemies.
	AActor* ThisTickActor = CursorHit.GetActor();
	if (LastTickActor != ThisTickActor)
	{
		ToggleHighlight(LastTickActor, false);
		ToggleHighlight(ThisTickActor, true);
	}
	LastTickActor = ThisTickActor;
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

void AP16PlayerController::UpdateMagicCircle() const
{
	EARLY_RETURN_IF(!MagicCircle || !CursorHit.bBlockingHit)

	// Remove magic circle jitter on hovering the enemies, by ignoring them in additional raycast.
	FVector MagicCircleLocation = CursorHit.ImpactPoint;
	if (LastTickActor)
	{
		FHitResult    IgnoreEnemyHit;
		const FVector Start     = CursorHit.ImpactPoint;
		const FVector Direction = (CursorHit.TraceEnd - CursorHit.TraceStart).GetSafeNormal();
		const FVector End       = Start + Direction * 1000.f;
		if (GetWorld()->LineTraceSingleByChannel(IgnoreEnemyHit, Start, End, P16::CollisionChannel::Cursor))
		{
			MagicCircleLocation = IgnoreEnemyHit.ImpactPoint;
		}
	}

	MagicCircle->SetActorLocation(MagicCircleLocation);
}

void AP16PlayerController::AbilityInputTagPressed(const FGameplayTag InputTag)
{
	EARLY_RETURN_IF(!GetAbilitySystemComponent()
		|| GetAbilitySystemComponent()->HasMatchingGameplayTag(FGSGameplayTagsSingleton::Get().P16Tags.Player.Blocked.InputPressedTag))

	if (GetIsLMB(InputTag))
	{
		TargetingStatus = GetTargetingStatus(LastTickActor);
		bAutoRunning    = false;
		FollowTime      = 0.f;
	}

	EARLY_RETURN_IF(!GetAbilitySystemComponent())
	AbilitySystemComponent->AbilityInputTagPressed(InputTag);
}

void AP16PlayerController::AbilityInputTagReleased(const FGameplayTag InputTag)
{
	EARLY_RETURN_IF(!GetAbilitySystemComponent()
		|| GetAbilitySystemComponent()->HasMatchingGameplayTag(FGSGameplayTagsSingleton::Get().P16Tags.Player.Blocked.InputReleasedTag))

	// Activate ability.
	const bool bTargetingEnemy = TargetingStatus == EP16TargetingStatus::Enemy;
	if (bTargetingEnemy || !GetIsLMB(InputTag))
	{
		EARLY_RETURN_IF(!GetAbilitySystemComponent())
		AbilitySystemComponent->AbilityInputTagReleased(InputTag);
		return;
	}

	// Move towards click (!) behavior.
	EARLY_RETURN_IF(FollowTime > ClickThreshold || !ControlledPawn)

	// Either replace the click location with the target's destination, or show the click effect.
	if (LastTickActor && LastTickActor->Implements<UP16HighlightInterface>())
	{
		IP16HighlightInterface::Execute_UpdateDestination(LastTickActor, OUT CachedDestination);
	}
	else if (!GetAbilitySystemComponent()->HasMatchingGameplayTag(FGSGameplayTagsSingleton::Get().P16Tags.Player.Blocked.InputPressedTag))
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ClickNiagara, CachedDestination);
	}

	// Convert found navigation path points to smooth spline.
	UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination);
	EARLY_RETURN_IF(!NavPath)

	Spline->ClearSplinePoints();
	for (const FVector& PathPoint : NavPath->PathPoints)
	{
		Spline->AddSplinePoint(PathPoint, ESplineCoordinateSpace::World, false);
		CachedDestination = PathPoint;
	}
	bAutoRunning = !NavPath->PathPoints.IsEmpty();
}

void AP16PlayerController::AbilityInputTagHeld(const FGameplayTag InputTag)
{
	EARLY_RETURN_IF(!GetAbilitySystemComponent()
		|| GetAbilitySystemComponent()->HasMatchingGameplayTag(FGSGameplayTagsSingleton::Get().P16Tags.Player.Blocked.InputHeldTag))

	// Activate ability.
	const bool bTargetingEnemy = TargetingStatus == EP16TargetingStatus::Enemy;
	if (!GetIsLMB(InputTag) || bTargetingEnemy || bShiftKeyDown)
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
	check(MappingContext);

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
	return InputTag.MatchesTagExact(FGSGameplayTagsSingleton::Get().P16Tags.Input.MouseButtonLeftTag);
}

void AP16PlayerController::ToggleHighlight(AActor* Target, const bool bOn)
{
	EARLY_RETURN_IF(!Target || !Target->Implements<UP16HighlightInterface>())
	IP16HighlightInterface::Execute_ToggleHighlight(Target, bOn);
}

EP16TargetingStatus AP16PlayerController::GetTargetingStatus(AActor* Target)
{
	EARLY_RETURN_VALUE_IF(!Target || !Target->Implements<UP16HighlightInterface>(), EP16TargetingStatus::None)
	return IP16HighlightInterface::Execute_GetTargetingStatus(Target);
}
