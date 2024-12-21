// Copyright (C) 2023, IKinder

#include "Weapon/ARProjectileMagic.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h" 
#include "Ability/ARAbilityEffect.h" // ReSharper disable once CppUnusedIncludeDirective
#include "Component/ARAbilityComponent.h"
#include "Util/ARFuncLibrary.h"

AARProjectileMagic::AARProjectileMagic()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AARProjectileMagic::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(10.f);
	MovementComp->bIsHomingProjectile = true;
	// MovementComp->HomingAccelerationMagnitude = 5000.f;
}

void AARProjectileMagic::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Target) return;
	const float CurrentDistance = FVector::Dist(GetActorLocation(), Target->GetActorLocation());
	if (FMath::IsNearlyZero(CurrentDistance)) return;

	MovementComp->HomingAccelerationMagnitude = 100.f / CurrentDistance * 100000;
	// GEngine->AddOnScreenDebugMessage(-1, -1.f, FColor::Cyan, FString::Printf(TEXT("%f"), MovementComp->HomingAccelerationMagnitude));
}

void AARProjectileMagic::AddActorToIgnore(AActor* Actor)
{
	SphereComp->IgnoreActorWhenMoving(Actor, true);
}

void AARProjectileMagic::SetTarget(AActor* TheTarget)
{
	if (!TheTarget) return;

	Target = TheTarget;
	MovementComp->HomingTargetComponent = Target->FindComponentByClass<USceneComponent>();
}

void AARProjectileMagic::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (Target)
	{
		Target->Destroy();
	}
	Super::OnProjectileHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
}

void AARProjectileMagic::OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == GetInstigator()) return;

	UARAbilityComponent* AbilityComp = OtherActor->FindComponentByClass<UARAbilityComponent>();
	if (AbilityComp && AbilityComp->ActiveGameplayTags.HasTag(ParryTag))
	{
		MovementComp->Velocity *= -1;
		SetInstigator(Cast<APawn>(OtherActor));
		return;
	}

	if (UARFuncLibrary::ApplyDamage(GetInstigator(), OtherActor, Damage))
	{
		if (AbilityComp && HasAuthority())
		{
			AbilityComp->AddAbility(GetInstigator(), BurningEffectClass);
		}
		Explode();
	}
}

void AARProjectileMagic::Explode()
{
	Super::Explode();
}
