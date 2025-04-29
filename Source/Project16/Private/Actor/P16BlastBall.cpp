// Copyright © 2025, Ivan Kinder

#include "Actor/P16BlastBall.h"

#include "AbilitySystem/P16AbilitySystemLibrary.h"
#include "AbilitySystem/Ability/P16DamageGameplayAbility.h"
#include "Components/AudioComponent.h"

AP16BlastBall::AP16BlastBall()
{}

void AP16BlastBall::BeginPlay()
{
	Super::BeginPlay();

	Start = GetInstigator()->GetActorLocation();
	End   = Start + GetActorForwardVector() * Distance;
}

void AP16BlastBall::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP16BlastBall::Destroyed()
{
	// Update radial damage origin and find all live targets around it.
	DamageEffectParams.RadialParams.bRadial = true;
	const FVector Origin                    = GetInstigator() ? GetInstigator()->GetActorLocation() : GetActorLocation();
	UP16DamageGameplayAbility::UpdateRadialDamageOrigin(Origin, DamageEffectParams.RadialParams);
	TArray<AActor*> Targets = UP16AbilitySystemLibrary::GetLivePlayersWithinRadius(
		this,
		{GetInstigator()},
		DamageEffectParams.RadialParams.OuterRadius,
		DamageEffectParams.RadialParams.Origin);

	// Apply damage to all targets.
	for (AActor* Target : Targets)
	{
		if (HasAuthority())
		{
			ApplyDamageTo(Target);
		}
	}

	Super::Destroyed();
}

void AP16BlastBall::OnSphereBeginOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority())
	{
		ApplyDamageTo(OtherActor);
	}
}

void AP16BlastBall::CheckTarget()
{
	// Do nothing here, sorry Mrs Liskov.
}

void AP16BlastBall::PlayOnHitEffects() const
{
	PlayOnHitCue();

	if (LoopingSoundComp)
	{
		LoopingSoundComp->Stop();
	}
}
