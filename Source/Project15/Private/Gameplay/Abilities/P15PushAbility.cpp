// Copyright © 2025, Ivan Kinder

#include "Gameplay/Abilities/P15PushAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Project15.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "Gameplay/Effects/P15MeleeEffect.h"
#include "Player/P15Character.h"

UP15PushAbility::UP15PushAbility()
{
	CooldownGameplayEffectClass = UP15MeleeEffect::StaticClass();
}

void UP15PushAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo, nullptr))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
	}

	// Find an enemy.
	FHitResult            HitResult;
	const FVector         Start = Char->GetPlayerEye()->GetComponentLocation();
	const FVector         End   = Start + Char->GetPlayerEye()->GetForwardVector() * PushDistance;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Char);
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Pawn, Params);
	DrawDebug(HitResult);
	EARLY_RETURN_IF(!HitResult.bBlockingHit)
	AP15Character* Enemy = Cast<AP15Character>(HitResult.GetActor());
	EARLY_RETURN_IF(!Enemy || Enemy->GetIsDead())

	// Rotate the character to face the enemy.
	const FVector EnemyDirection = (Enemy->GetActorLocation() - Char->GetActorLocation()).GetSafeNormal();
	const float   DeltaYaw       = EnemyDirection.Rotation().Yaw - Char->GetActorForwardVector().Rotation().Yaw;
	Char->AddActorWorldRotation(FRotator(0.f, DeltaYaw, 0.f));

	// Play an animation.
	Char->PlayAnimMontage(PushMontage);
	Enemy->PlayAnimMontage(ReactMontage, 1.f, FindDirectionName(Enemy, Char));

	// Apply damage to enemy.
	ApplyGameplayEffectToTarget(
		CurrentSpecHandle,
		CurrentActorInfo,
		CurrentActivationInfo,
		UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(Enemy),
		DamageGameplayEffectClass,
		1.f);
}

void UP15PushAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const bool bReplicateEndAbility, const bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UP15PushAbility::DrawDebug(const FHitResult& InResult) const
{
	EARLY_RETURN_IF(!bDebug)
	DrawDebugLine(GetWorld(), InResult.TraceStart, InResult.TraceEnd, FColor::Green, false, 5.f);
}

FName UP15PushAbility::FindDirectionName(const AActor* Target, const AActor* Culprit) const
{
	using FDirection = TTuple<float, FName>;

	const FVector TargetDirection    = Target->GetActorForwardVector();
	const FVector ToCulpritDirection = (Culprit->GetActorLocation() - Target->GetActorLocation()).GetSafeNormal();
	const float   AngleBetween       = FMath::UnwindDegrees(TargetDirection.Rotation().Yaw - ToCulpritDirection.Rotation().Yaw);

	const TArray<FDirection> Directions = {{0.f, "Front"}, {90.f, "Left"}, {-90.f, "Right"}, {180.f, "Back"}};
	const FDirection*        Result     = Algo::FindByPredicate(Directions, [AngleBetween](const FDirection Pair) -> bool
	{
		const float Min = Pair.Key - 45.f;
		const float Max = Pair.Key + 45.f;
		return FMath::IsWithin(AngleBetween, Min, Max);
	});

	return Result ? Result->Value : "Back";
}
