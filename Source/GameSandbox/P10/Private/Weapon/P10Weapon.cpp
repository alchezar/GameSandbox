// Copyright (C) 2023, IKinder

#include "P10/Public/Weapon/P10Weapon.h"

#include "GameSandbox.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "HAL/IConsoleManager.h"
#include "Kismet/GameplayStatics.h"
#include "P10/Public/Player/P10Character.h"
#include "P10/Public/Util/P10Library.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

AP10Weapon::AP10Weapon()
{
	PrimaryActorTick.bCanEverTick = false;

	RootOffsetComponent = CreateDefaultSubobject<USceneComponent>("RootOffsetSceneComponent");
	SetRootComponent(RootOffsetComponent);
	
	WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponSkeletalMeshComponent");
	WeaponMeshComponent->SetupAttachment(RootComponent);
	WeaponMeshComponent->SetRelativeLocationAndRotation(LocationOffset, RotationOffset);

	if (!FireSound)
	{
		static ConstructorHelpers::FObjectFinder<USoundBase> FireSoundFinder (TEXT("/Script/Engine.SoundCue'/Game/Project/P2/Sounds/Weapon/Blaster/SCue_BlasterFire.SCue_BlasterFire'"));
		if (FireSoundFinder.Succeeded()) FireSound = FireSoundFinder.Object;
	}
	if (!FireEffect)
	{
		static ConstructorHelpers::FObjectFinder<UNiagaraSystem> FireEffectFinder (TEXT("/Script/Niagara.NiagaraSystem'/Game/Project/P2/VFX/Muzzles/NS_BlasterMuzzle.NS_BlasterMuzzle'"));
		if (FireEffectFinder.Succeeded()) FireEffect = FireEffectFinder.Object;
	}
	if (!BeamEffect)
	{
		static ConstructorHelpers::FObjectFinder<UNiagaraSystem> BeamEffectFinder (TEXT("/Script/Niagara.NiagaraSystem'/Game/Project/PP10/Effect/NS_BlasterBeam_P10.NS_BlasterBeam_P10'"));
		if (BeamEffectFinder.Succeeded()) BeamEffect = BeamEffectFinder.Object;
	}
}

void AP10Weapon::BeginPlay()
{
	Super::BeginPlay();
	check(FireSound)
	check(FireEffect)
	
	Ammo = ClipCapacity;
	AmmoInClips = MaxAmmo - ClipCapacity;
}

void AP10Weapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP10Weapon::StartFire()
{
	if (FireMode > EP10FireMode::Single)
	{
		GetWorld()->GetTimerManager().SetTimer(FireTimer, this, &ThisClass::OneShot, TimeBetweenShots, true, TimeBetweenShots);
	}
	OneShot();
}

void AP10Weapon::OneShot()
{
	if (!bInfinite && --Ammo <= 0 && !TryReload()) return;
	
	/* Line trace the world, from pawn eyes to crosshair location */
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	check(OwnerPawn)
	const float HalfRad = FMath::DegreesToRadians(Spread / 2);
	const FVector Direction = FMath::VRandCone(OwnerPawn->GetControlRotation().Vector(), HalfRad);
	const FVector Start = OwnerPawn->GetPawnViewLocation();
	const FVector End = Start + Direction * 10000.f;
		
	FHitResult Hit;
	TArray<AActor*> Actors = {OwnerPawn, this};	
	FCollisionQueryParams Params;
	Params.AddIgnoredActors(Actors);
	Params.bTraceComplex = true;
	Params.bReturnPhysicalMaterial = true;
	
	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_PROJECTILE, Params);
	if (Hit.bBlockingHit && Hit.GetActor())
	{
		const bool bHeadshot = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get()) == SURFACE_HEAD;
		float Damage = bHeadshot ? BaseDamage * 2.f : BaseDamage;
		UGameplayStatics::ApplyPointDamage(Hit.GetActor(), Damage, Direction, Hit, OwnerPawn->GetInstigatorController(), this, nullptr);
		UP10Library::InteractWithPhysical(Hit.GetActor(), Hit.GetComponent(), this);
		UP10Library::DrawTargetInfo(this, Hit.ImpactPoint, FString::SanitizeFloat(Damage));
	}
	UP10Library::DrawDebugShoot(this, Hit);

	PlayMuzzleEffects();
	DrawBeam(Hit, End);
	PlayImpactEffect(Hit);

	/* Shot outcome */
	OwnerPawn->AddControllerPitchInput(-ShotRecoil);
	if (GetNextFireCount() >= 3 && FireMode == EP10FireMode::Brush)
	{
		StopFire();
	}
}

void AP10Weapon::StopFire()
{
	if (FireMode > EP10FireMode::Single)
	{
		GetWorld()->GetTimerManager().ClearTimer(FireTimer);
	}
	FireCount = 0;
}

bool AP10Weapon::TryReload()
{
	if (Ammo <= 0 && AmmoInClips <= 0)
	{
		Ammo = AmmoInClips = 0;
		
		OnReload.Broadcast(Cast<APawn>(GetOwner()), false);
		return false;
	}

	AmmoInClips += Ammo;
	Ammo = AmmoInClips > ClipCapacity ? ClipCapacity : AmmoInClips;
	AmmoInClips -= Ammo;
	
	OnReload.Broadcast(Cast<APawn>(GetOwner()), true);
	return true;
}

void AP10Weapon::PlayMuzzleEffects() const
{
	const FName MuzzleSocketName = WeaponMeshComponent->GetAllSocketNames()[0];
	const FVector MuzzleSocketLocation = WeaponMeshComponent->GetSocketLocation(MuzzleSocketName);
	
	UGameplayStatics::PlaySoundAtLocation(this, FireSound, MuzzleSocketLocation, FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAttached(FireEffect, WeaponMeshComponent, MuzzleSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true);

	if (ShakeClass)
	{
		const ACharacter* Character =  Cast<ACharacter>(GetOwner());
		if (!Character) return;
		APlayerController* PlayerController = Character->GetLocalViewingPlayerController();
		if (!PlayerController) return;

		PlayerController->ClientStartCameraShake(ShakeClass);
	}
}

void AP10Weapon::DrawBeam(const FHitResult& Hit, const FVector& End)
{
	if (!BeamEffect) return;

	const FVector MuzzleSocketLocation = WeaponMeshComponent->GetSocketLocation( WeaponMeshComponent->GetAllSocketNames()[0]);
	UNiagaraComponent* BeamNiagara = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, BeamEffect, MuzzleSocketLocation);
	if (!BeamNiagara) return;
	
	BeamNiagara->SetVariableVec3(TraceTargetName, Hit.bBlockingHit ? Hit.ImpactPoint : End);
	BeamNiagara->SetVariableLinearColor(FName("BlasterColor"), FColor::Red);
}

void AP10Weapon::PlayImpactEffect(const FHitResult& Hit)
{
	if (ImpactMap.IsEmpty()) return;
	
	UNiagaraSystem* ImpactEffect = ImpactMap[EPhysicalSurface::SurfaceType_Default];
	const EPhysicalSurface Surface = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
	
	if (ImpactMap.Contains(Surface))
	{
		ImpactEffect = ImpactMap[Surface];
	}
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, Hit.ImpactPoint);
}
