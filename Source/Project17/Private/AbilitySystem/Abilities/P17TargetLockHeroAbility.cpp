// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/Abilities/P17TargetLockHeroAbility.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Character/P17CharacterHero.h"
#include "Components/SizeBox.h"
#include "Controller/P17ControllerHero.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Project17/Project17.h"
#include "Util/P17FunctionLibrary.h"
#include "Util/P17GameplayTags.h"
#include "Widget/P17WidgetBase.h"

void UP17TargetLockHeroAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	LockOnTarget();
	ToggleTargetLockMovement(true);
}

void UP17TargetLockHeroAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const bool bReplicateEndAbility, const bool bWasCancelled)
{
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

void UP17TargetLockHeroAbility::LockOnTarget()
{
	GetAvailableActorsToLock();

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

void UP17TargetLockHeroAbility::GetAvailableActorsToLock()
{
	const AActor* Avatar = GetAvatarActorFromActorInfo();
	const FVector Forward = Avatar->GetActorForwardVector();
	const FVector Start = Avatar->GetActorLocation();
	const FVector End = Start + Forward * TraceDistance;
	const EDrawDebugTrace::Type DebugType = bShowDebug ? EDrawDebugTrace::Persistent : EDrawDebugTrace::None;

	TArray<FHitResult> HitResults;
	UKismetSystemLibrary::BoxTraceMultiForObjects(
		Avatar,
		Start,
		End,
		TraceBoxSize / 2,
		Forward.Rotation(),
		TraceChannels,
		false,
		{},
		DebugType,
		HitResults,
		true);

	for (const FHitResult& HitResult : HitResults)
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
	FaceControllerTo(CurrentLockedActor, DeltaTime, RotationInterpSpeed);
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

	if (bOn && FMath::IsNearlyZero(DefaultWalkSpeed))
	{
		DefaultWalkSpeed = Movement->MaxWalkSpeed;
	}

	Movement->MaxWalkSpeed = bOn ? MaxWalkSpeed : DefaultWalkSpeed;
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
