// Copyright (C) 2023, IKinder

#include "P10/Public/Weapon/P10Weapon.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "P10/Public/Util/P10Library.h"

AP10Weapon::AP10Weapon()
{
	PrimaryActorTick.bCanEverTick = true;

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
}

void AP10Weapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP10Weapon::StartFire()
{
	/* Line trace the world, from pawn eyes to crosshair location */
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	check(OwnerPawn)
	const FVector Direction = OwnerPawn->GetControlRotation().Vector();
	const FVector Start = OwnerPawn->GetPawnViewLocation();
	const FVector End = Start + Direction * 10000.f;
		
	FHitResult Hit;
	FCollisionQueryParams Params;
	TArray<AActor*> Actors = {OwnerPawn, this};	
	Params.AddIgnoredActors(Actors);
	Params.bTraceComplex = true;
	
	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);
	// DrawDebugShoot(Hit, Start, End);
	UP10Library::DrawDebugShoot(this, Hit);
	if (Hit.bBlockingHit && Hit.GetActor())
	{
		UGameplayStatics::ApplyPointDamage(Hit.GetActor(), 10.f, Direction, Hit, OwnerPawn->GetInstigatorController(), this, nullptr);
		UP10Library::InteractWithPhysical(Hit.GetActor(), Hit.GetComponent(), this);
	}

	PlayEffect();
	DrawBeam(Hit, End);
}

void AP10Weapon::PlayEffect() const
{
	const FName MuzzleSocketName = WeaponMeshComponent->GetAllSocketNames()[0];
	const FVector MuzzleSocketLocation = WeaponMeshComponent->GetSocketLocation(MuzzleSocketName);
	
	UGameplayStatics::PlaySoundAtLocation(this, FireSound, MuzzleSocketLocation, FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAttached(FireEffect, WeaponMeshComponent, MuzzleSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true);
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