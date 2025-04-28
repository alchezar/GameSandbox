// Copyright © 2025, Ivan Kinder

#include "Actor/P16BlastBall.h"

#include "AbilitySystemComponent.h"
#include "Project16.h"
#include "AbilitySystem/P16AbilitySystemLibrary.h"

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

void AP16BlastBall::OnSphereBeginOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	EARLY_RETURN_IF(!DamageEffectParams.SourceAbilitySystemComponent)
	const AActor* SourceAvatar = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	EARLY_RETURN_IF(SourceAvatar == OtherActor)

	if (HasAuthority())
	{
		ApplyDamageTo(OtherActor);
	}
}

void AP16BlastBall::CustomMove(const float DeltaSeconds)
{}

void AP16BlastBall::CheckTarget()
{}
