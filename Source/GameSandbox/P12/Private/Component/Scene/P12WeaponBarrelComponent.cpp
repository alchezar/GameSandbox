// Copyright (C) 2023, IKinder

#include "P12/Public/Component/Scene/P12WeaponBarrelComponent.h"

#include "GameSandbox.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/DecalComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "P12/Public/Util/P12Library.h"

UP12WeaponBarrelComponent::UP12WeaponBarrelComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UP12WeaponBarrelComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UP12WeaponBarrelComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UP12WeaponBarrelComponent::Shot(const FVector& ShotStart, const FVector& ShotDirection, AController* Instigator)
{
	const FVector MuzzleLocation = GetComponentLocation();
	const FVector ShotEnd = ShotStart + FiringRange * ShotDirection;
	
	FHitResult ShotHitResult;
	GetWorld()->LineTraceSingleByChannel(ShotHitResult, MuzzleLocation, ShotEnd, ECC_BULLET);
	UP12Library::DrawDebugLineTrace(GetWorld(), ShotHitResult, UP12Library::GetCanDrawDebugFire(), false);
	if (ShotHitResult.bBlockingHit)
	{
		/* Take Damage. */
		AActor* HitActor = ShotHitResult.GetActor();
		if (!HitActor)
		{
			return;
		}
		HitActor->TakeDamage(DamageAmount, FDamageEvent(), Instigator, GetOwner());
		
		/* Draw decal. */
		if (UDecalComponent* BulletHoleDecal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), DecalInfo.Decal, DecalInfo.Size, ShotHitResult.ImpactPoint, ShotHitResult.ImpactNormal.ToOrientationRotator()))
		{
			BulletHoleDecal->SetFadeOut(DecalInfo.Lifetime, DecalInfo.FadeOutTime);
			BulletHoleDecal->SetFadeScreenSize(DecalInfo.FadeScreenSize);
		}
		
	}

	/* Trace tale Niagara. */
	if (TraceNiagara)
	{
		const FVector NiagaraLocation = ShotHitResult.bBlockingHit ? ShotHitResult.ImpactPoint : ShotHitResult.TraceEnd;
		UNiagaraComponent* TraceNiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceNiagara, MuzzleLocation);
		if (!TraceNiagaraComp)
		{
			return;
		}
		TraceNiagaraComp->SetVectorParameter("TraceEnd", NiagaraLocation);
	}
}
