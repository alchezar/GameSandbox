// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/Abilities/Traceable/P17TargetLockHeroAbility.h"

#include "EnhancedInputSubsystems.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Character/P17CharacterHero.h"
#include "Components/SizeBox.h"
#include "Controller/P17ControllerHero.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Project17/Project17.h"
#include "Util/P17FunctionLibrary.h"
#include "Util/P17GameplayTags.h"
#include "Widget/P17WidgetBase.h"

UP17TargetLockHeroAbility::UP17TargetLockHeroAbility()
{
	TraceDistance = 5000.f;
	TraceBoxSize = FVector {2000.f, 2000.f, 300.f};
	TraceChannels = {ObjectTypeQuery3};
}

void UP17TargetLockHeroAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ToggleTargetLockMappingContext(true);
	ToggleTargetLockMovement(true);
	LockOnTarget();
}

void UP17TargetLockHeroAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const bool bReplicateEndAbility, const bool bWasCancelled)
{
	ToggleTargetLockMappingContext(false);
	ToggleTargetLockMovement(false);
	CleanUp();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UP17TargetLockHeroAbility::OnTargetLockTick(const float DeltaTime)
{
	if (!CurrentLockedActor
		|| UP17FunctionLibrary::NativeGetActorHasTag(CurrentLockedActor, P17::Tags::Shared_Status_Dead)
		|| UP17FunctionLibrary::NativeGetActorHasTag(GetAvatarActorFromActorInfo(), P17::Tags::Shared_Status_Dead))
	{
		CancelTargetLock();
		return;
	}

	SetTargetLockWidgetPosition();
	OrientToTarget(DeltaTime);
}

void UP17TargetLockHeroAbility::SwitchTarget(const FGameplayTag& InSwitchDirectionTag)
{
	FindAvailableActorsToLock();
	const FP17Neighbours Neighbours = GetNeighbours();

	if (Neighbours.Left && InSwitchDirectionTag.MatchesTagExact(P17::Tags::Player_Event_SwitchTarget_Left))
	{
		CurrentLockedActor = Neighbours.Left;
	}
	else if (Neighbours.Right && InSwitchDirectionTag.MatchesTagExact(P17::Tags::Player_Event_SwitchTarget_Right))
	{
		CurrentLockedActor = Neighbours.Right;
	}
}

void UP17TargetLockHeroAbility::LockOnTarget()
{
	FindAvailableActorsToLock();

	if (ActorsToLock.IsEmpty())
	{
		CancelTargetLock();
		return;
	}
	CurrentLockedActor = GetNearestTarget();
	WARN_RETURN_IF(!CurrentLockedActor,)

	DrawTargetLockWidget();
	FindWidgetSize();
	SetTargetLockWidgetPosition();
}

void UP17TargetLockHeroAbility::FindAvailableActorsToLock()
{
	ActorsToLock.Empty();
	for (const FHitResult& HitResult : BoxTrace())
	{
		AActor* HitActor = HitResult.GetActor();
		CONTINUE_IF(!HitActor || HitActor == GetAvatarActorFromActorInfo())

		ActorsToLock.AddUnique(HitActor);
	}
}

AActor* UP17TargetLockHeroAbility::GetNearestTarget()
{
	const FVector Origin = GetAvatarActorFromActorInfo()->GetActorLocation();

	AActor** Nearest = Algo::MinElementBy(ActorsToLock, [Origin](const AActor* Actor) -> float
	{
		return FVector::DistSquared(Actor->GetActorLocation(), Origin);
	});

	return Nearest ? *Nearest : nullptr;
}

FP17Neighbours UP17TargetLockHeroAbility::GetNeighbours()
{
	FP17Neighbours Result = {};

	if (ActorsToLock.IsEmpty())
	{
		CancelTargetLock();
		return Result;
	}

	const AActor* Avatar = GetAvatarActorFromActorInfo();
	WARN_RETURN_IF(!Avatar || !CurrentLockedActor, Result)

	// Sort actors array by angle.
	const FVector PlayerLocation = Avatar->GetActorLocation();
	const FVector PlayerDirection = (CurrentLockedActor->GetActorLocation() - PlayerLocation).GetSafeNormal2D();
	Algo::SortBy(ActorsToLock, [PlayerLocation, PlayerDirection](const AActor* Target) -> float
	{
		const FVector ToTargetDirection = (Target->GetActorLocation() - PlayerLocation).GetSafeNormal2D();
		const float Angle = FMath::RadiansToDegrees(FMath::Acos(PlayerDirection | ToTargetDirection));
		const float Sign = FMath::Sign((PlayerDirection ^ ToTargetDirection).Z);

		return Angle * Sign;
	});

	// Find locked actor's index and his left and right neighbours.
	const int32 IndexFound = ActorsToLock.IndexOfByPredicate([this](const AActor* Target) -> bool
	{
		return Target == CurrentLockedActor;
	});
	WARN_RETURN_IF(IndexFound == INDEX_NONE, Result)

	const int32 IndexLeft = IndexFound - 1;
	const int32 IndexRight = IndexFound + 1;
	const int32 IndexLast = ActorsToLock.Num() - 1;

	Result.Left = IndexLeft >= 0 ? ActorsToLock[IndexLeft] : nullptr;
	Result.Right = IndexRight <= IndexLast ? ActorsToLock[IndexRight] : nullptr;
	return Result;
}

void UP17TargetLockHeroAbility::DrawTargetLockWidget()
{
	WARN_RETURN_IF(!WidgetClass || TargetLockWidget,)
	TargetLockWidget = CreateWidget<UP17WidgetBase>(GetHeroControllerFromActorInfo(), WidgetClass);
	WARN_RETURN_IF(!TargetLockWidget,)

	TargetLockWidget->AddToViewport();
}

void UP17TargetLockHeroAbility::SetTargetLockWidgetPosition()
{
	if (!CurrentLockedActor || !TargetLockWidget)
	{
		CancelTargetLock();
		return;
	}

	FVector2D ScreenPosition {};
	UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(
		GetHeroControllerFromActorInfo(),
		CurrentLockedActor->GetActorLocation(),
		ScreenPosition,
		true);
	ScreenPosition -= TargetLockSize / 2;

	TargetLockWidget->SetPositionInViewport(ScreenPosition, false);
}

void UP17TargetLockHeroAbility::OrientToTarget(const float DeltaTime) const
{
	const bool bRolling = UP17FunctionLibrary::NativeGetActorHasTag(GetAvatarActorFromActorInfo(), P17::Tags::Player_Status_Rolling);
	const bool bBlocking = UP17FunctionLibrary::NativeGetActorHasTag(GetAvatarActorFromActorInfo(), P17::Tags::Player_Status_Blocking);
	RETURN_IF(bRolling || bBlocking,)

	FaceOwnerTo(CurrentLockedActor);
	FaceControllerTo(CurrentLockedActor, DeltaTime, RotationInterpSpeed, CameraOffset);
}

void UP17TargetLockHeroAbility::FindWidgetSize()
{
	WARN_RETURN_IF(!TargetLockWidget,)
	RETURN_IF(TargetLockSize != FVector2D::ZeroVector,)

	TargetLockWidget->WidgetTree->ForEachWidget([this](UWidget* FoundWidget) -> void
	{
		const USizeBox* FoundSizeBox = Cast<USizeBox>(FoundWidget);
		RETURN_IF(!FoundSizeBox,)
		TargetLockSize.X = FoundSizeBox->GetWidthOverride();
		TargetLockSize.Y = FoundSizeBox->GetHeightOverride();
	});
}

void UP17TargetLockHeroAbility::ToggleTargetLockMovement(const bool bOn)
{
	const AP17CharacterHero* Char = GetHeroCharacterFromActorInfo();
	WARN_RETURN_IF(!Char,)
	UCharacterMovementComponent* Movement = Char->GetCharacterMovement();
	WARN_RETURN_IF(!Movement,)

	if (bOn)
	{
		DefaultWalkSpeed = Movement->MaxWalkSpeed;
	}

	RETURN_IF(FMath::IsNearlyZero(DefaultWalkSpeed),)
	Movement->MaxWalkSpeed = bOn ? MaxWalkSpeed : DefaultWalkSpeed;
}

void UP17TargetLockHeroAbility::ToggleTargetLockMappingContext(const bool bOn)
{
	WARN_RETURN_IF(!TargetLockMappingContext,)
	const AP17ControllerHero* Controller = GetHeroControllerFromActorInfo();
	WARN_RETURN_IF(!Controller,)
	const ULocalPlayer* LocalPlayer = Controller->GetLocalPlayer();
	WARN_RETURN_IF(!LocalPlayer,)
	auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	WARN_RETURN_IF(!Subsystem,)

	if (bOn)
	{
		Subsystem->AddMappingContext(TargetLockMappingContext, 2);
	}
	else
	{
		Subsystem->RemoveMappingContext(TargetLockMappingContext);
	}
}

void UP17TargetLockHeroAbility::CancelTargetLock()
{
	CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
}

void UP17TargetLockHeroAbility::CleanUp()
{
	ActorsToLock.Empty();
	CurrentLockedActor = nullptr;
	if (TargetLockWidget)
	{
		TargetLockWidget->RemoveFromParent();
		TargetLockWidget = nullptr;
	}
	TargetLockSize = FVector2D::ZeroVector;
	DefaultWalkSpeed = 0.f;
}
