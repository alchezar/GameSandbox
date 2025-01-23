// Copyright (C) 2023, IKinder

#include "Weapon/P10Weapon.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "HAL/IConsoleManager.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Player/P10Character.h"
#include "Project10/Project10.h"
#include "Util/P10Library.h"

AP10Weapon::AP10Weapon()
{
	PrimaryActorTick.bCanEverTick = false;
	// SetReplicates(true);
	bReplicates = true;
	SetNetUpdateFrequency(60.f);
	SetMinNetUpdateFrequency(30.f);

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

void AP10Weapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(ThisClass, HitScanTrace, COND_SkipOwner)
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
	/* Shoot only if there are enough ammo in the magazine. */
	if (!bInfinite && --Ammo <= 0 && !TryReload()) return;
	
	/* Line trace the world, from pawn eyes to crosshair location */
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	check(OwnerPawn)
	const float HalfRad = FMath::DegreesToRadians(Spread / 2);
	const FVector Direction = FMath::VRandCone(OwnerPawn->GetControlRotation().Vector(), HalfRad);
	const FVector Start = OwnerPawn->GetPawnViewLocation();
	const FVector End = Start + Direction * 10000.f;

	/* Init default values for effects played on the server and other clients. */
	EPhysicalSurface Surface = EPhysicalSurface::SurfaceType_Default;
	FVector Target = End;

	/* Init params for line trace and its execution. */
	FHitResult Hit;
	if (TryToPerformLineTrace(Hit, OwnerPawn, Start, End))
	{
		const bool bHeadshot = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get()) == SURFACE_HEAD;
		float Damage = bHeadshot ? BaseDamage * 2.f : BaseDamage;
		UGameplayStatics::ApplyPointDamage(Hit.GetActor(), Damage, Direction, Hit, OwnerPawn->GetInstigatorController(), OwnerPawn, nullptr);
		UP10Library::InteractWithPhysical(Hit.GetActor(), Hit.GetComponent(), this);
		UP10Library::DrawTargetInfo(this, Hit.ImpactPoint, FString::SanitizeFloat(Damage));

		Surface = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
		Target = Hit.ImpactPoint;
	}
	UP10Library::DrawDebugShoot(this, Hit);

	PlayAllEffects(Target, Surface);
	ShotConsequences(OwnerPawn);

	/* Server side. */
	if (!HasAuthority())
	{
		Server_Fire();
	}
	else
	{
		HitScanTrace = {Surface, Target};
	}
}

void AP10Weapon::Server_Fire_Implementation()
{
	OneShot();
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

void AP10Weapon::DrawBeam(const FVector& End)
{
	if (!BeamEffect) return;

	const FVector MuzzleSocketLocation = WeaponMeshComponent->GetSocketLocation( WeaponMeshComponent->GetAllSocketNames()[0]);
	UNiagaraComponent* BeamNiagara = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, BeamEffect, MuzzleSocketLocation);
	if (!BeamNiagara) return;
	
	BeamNiagara->SetVariableVec3(TraceTargetName, End);
	BeamNiagara->SetVariableLinearColor(FName("BlasterColor"), FColor::Red);
}

void AP10Weapon::PlayImpactEffect(const EPhysicalSurface Surface, const FVector& End)
{
	if (ImpactMap.IsEmpty()) return;
	
	UNiagaraSystem* ImpactEffect = ImpactMap[EPhysicalSurface::SurfaceType_Default];
	if (ImpactMap.Contains(Surface))
	{
		ImpactEffect = ImpactMap[Surface];
	}
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, End);
}

void AP10Weapon::OnRep_HitScanTrace()
{
	PlayMuzzleEffects();
	DrawBeam(HitScanTrace.TraceTo);
	PlayImpactEffect(HitScanTrace.SurfaceType, HitScanTrace.TraceTo);
}

bool AP10Weapon::TryToPerformLineTrace(FHitResult& Hit, APawn* OwnerPawn, const FVector& Start, const FVector& End)
{
	const TArray<AActor*> Actors = {OwnerPawn, this};	
	FCollisionQueryParams Params;
	Params.AddIgnoredActors(Actors);
	Params.bTraceComplex = true;
	Params.bReturnPhysicalMaterial = true;
	
	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_PROJECTILE, Params);
	return Hit.bBlockingHit && Hit.GetActor();
}

void AP10Weapon::PlayAllEffects(const FVector& Target, const EPhysicalSurface Surface)
{
	PlayMuzzleEffects();
	DrawBeam(Target);
	PlayImpactEffect(Surface, Target);
}

void AP10Weapon::ShotConsequences(APawn* OwnerPawn)
{
	OwnerPawn->AddControllerPitchInput(-ShotRecoil);
	
	if (GetNextFireCount() >= 3 && FireMode == EP10FireMode::Brush)
	{
		StopFire();
	}
}
