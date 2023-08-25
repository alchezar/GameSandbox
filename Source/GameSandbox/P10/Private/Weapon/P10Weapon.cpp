// Copyright (C) 2023, IKinder

#include "P10/Public/Weapon/P10Weapon.h"

#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

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
	DrawDebugShoot(Hit, Start, End);
	if (!Hit.bBlockingHit) return;

	if (Hit.GetActor())
	{
		UGameplayStatics::ApplyPointDamage(Hit.GetActor(), 10.f, Direction, Hit, OwnerPawn->GetInstigatorController(), this, nullptr);
	}

	/* Play effects */
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, WeaponMeshComponent->GetComponentLocation(), FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAttached(FireEffect, WeaponMeshComponent, WeaponMeshComponent->GetAllSocketNames()[0], FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true);
}

void AP10Weapon::DrawDebugShoot(const FHitResult& OutHit, const FVector& Start, const FVector& End) const
{
	if (OutHit.bBlockingHit)
	{
		DrawDebugLine(GetWorld(), Start, OutHit.ImpactPoint, FColor::Red, false, 10.f);
		DrawDebugLine(GetWorld(), OutHit.ImpactPoint, End, FColor::Green, false, 10.f);
		DrawDebugPoint(GetWorld(), OutHit.ImpactPoint, 10.f, FColor::Red, false, 10.f);
	}
	else
	{
		DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 10.f);
	}
}
