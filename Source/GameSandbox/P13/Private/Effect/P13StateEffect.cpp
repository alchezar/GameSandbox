// Copyright © 2024, IKinder

#include "P13/Public/Effect/P13StateEffect.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "P13/Public/Component/Actor/P13HealthComponent.h"
#include "P13/Public/Intearface/P13StateEffectInterface.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                          State Effect Base Class                          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

UP13StateEffect::UP13StateEffect()
{}

void UP13StateEffect::InitObject(AActor* NewActor)
{
	if (!NewActor)
	{
		return;
	}
	if (IP13StateEffectInterface* EffectInterface = Cast<IP13StateEffectInterface>(NewActor))
	{
		EffectInterface->AddActiveStateEffect(this);
	}

	CachedActor = NewActor;
}

void UP13StateEffect::DestroyObject()
{
	if (!this || !this->IsValidLowLevel())
	{
		return;
	}

	if (IP13StateEffectInterface* EffectInterface = Cast<IP13StateEffectInterface>(CachedActor))
	{
		EffectInterface->RemoveInactiveStateEffect(this);
	}

	CachedActor.Reset();
	ConditionalBeginDestroy();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                       State Effect Single Execute                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void UP13SingleStateEffect::InitObject(AActor* NewActor)
{
	Super::InitObject(NewActor);
	StartEffect();
}

void UP13SingleStateEffect::DestroyObject()
{
	Super::DestroyObject();
}

void UP13SingleStateEffect::StartEffect()
{
	ParticleComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(Particle, CachedActor->GetRootComponent(), NAME_None, FVector::ZeroVector, FRotator::ZeroRotator, FVector(4.f), EAttachLocation::SnapToTarget, true, ENCPoolMethod::None);

	ApplyEffect();
	DestroyObject();
}

void UP13SingleStateEffect::ApplyEffect()
{
	check(CachedActor.IsValid())

	UP13HealthComponent* HealthComponent = CachedActor->FindComponentByClass<UP13HealthComponent>();
	if (!HealthComponent)
	{
		return;
	}
	HealthComponent->ChangeHealth(Power);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                         State Effect Timer Execute                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void UP13TimerStateEffect::InitObject(AActor* NewActor)
{
	Super::InitObject(NewActor);
}

void UP13TimerStateEffect::DestroyObject()
{
	GetWorld()->GetTimerManager().ClearTimer(RateTimer);

	if (ParticleComponent)
	{
		ParticleComponent->DestroyComponent();
	}

	Super::DestroyObject();
}

void UP13TimerStateEffect::StartEffect()
{
	Power /= Duration / Rate;

	GetWorld()->GetTimerManager().SetTimer(DurationTimer, this, &ThisClass::DestroyObject, Duration);
	GetWorld()->GetTimerManager().SetTimer(RateTimer, this, &ThisClass::ApplyEffect, Rate, true);

	USceneComponent* ActorRoot = CachedActor->GetRootComponent();
	ParticleComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(Particle, ActorRoot, NAME_None, FVector::ZeroVector, FRotator::ZeroRotator, FVector(2.f), EAttachLocation::SnapToTarget, false, ENCPoolMethod::None);
}

void UP13TimerStateEffect::ApplyEffect()
{
	if (!CachedActor.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(DurationTimer);
		DestroyObject();
		return;
	}

	Super::ApplyEffect();
}
