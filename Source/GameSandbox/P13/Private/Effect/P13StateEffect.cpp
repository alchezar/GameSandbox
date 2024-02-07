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

void UP13StateEffect::InitObject(AActor* NewActor, AController* NewCauser)
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
	CachedCauser = NewCauser;
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

void UP13SingleStateEffect::InitObject(AActor* NewActor, AController* NewCauser)
{
	Super::InitObject(NewActor, NewCauser);
	StartEffect();
}

void UP13SingleStateEffect::DestroyObject()
{
	Super::DestroyObject();
}

void UP13SingleStateEffect::StartEffect()
{
	SpawnEffectParticles(Particle, CachedActor->GetRootComponent());

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
	HealthComponent->Server_ChangeHealth(Power, CachedCauser.Get());
}

void UP13SingleStateEffect::SpawnEffectParticles(UNiagaraSystem* SystemTemplate, USceneComponent* AttachToComponent)
{
	ParticleComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
		SystemTemplate,
		AttachToComponent,
		NAME_None,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		FVector(Scale),
		EAttachLocation::SnapToTarget,
		bAutoDestroy,
		ENCPoolMethod::None);
}

void UP13SingleStateEffect::DestroyEffectParticles()
{
	if (ParticleComponent)
	{
		ParticleComponent->DestroyComponent();
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                         State Effect Timer Execute                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

UP13TimerStateEffect::UP13TimerStateEffect()
{
	Scale = 2.f;
	bAutoDestroy = false;
}

void UP13TimerStateEffect::InitObject(AActor* NewActor, AController* NewCauser)
{
	Super::InitObject(NewActor, NewCauser);
}

void UP13TimerStateEffect::DestroyObject()
{
	DestroyEffectParticles();

	if (const UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(RateTimer);
		World->GetTimerManager().ClearAllTimersForObject(this);
	}

	Super::DestroyObject();
}

void UP13TimerStateEffect::StartEffect()
{
	Power /= Duration / Rate;

	if (const UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(DurationTimer, this, &ThisClass::DestroyObject, Duration);
		World->GetTimerManager().SetTimer(RateTimer, this, &ThisClass::ApplyEffect, Rate, true);
	}

	SpawnEffectParticles(Particle, CachedActor->GetRootComponent());
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
