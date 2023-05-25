// Copyright (C) 2023, IKinder

#include "TG_Gun.h"
#include "TG_Projectile.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "P4/AnimNotify/TG_AN_Fire.h"
#include "P4/Player/TG_BaseCharacter.h"
// #include "Perception/AISense_Hearing.h"

ATG_Gun::ATG_Gun()
{
	PrimaryActorTick.bCanEverTick = true;

	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>("FP_Gun");
	// FP_Gun->SetOnlyOwnerSee(true);
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;

	FP_Muzzle = CreateDefaultSubobject<USceneComponent>("MuzzleLocation");
	FP_Muzzle->SetupAttachment(FP_Gun);
	FP_Muzzle->SetRelativeLocation(FVector(0.f, 60.f, 9.3f));
}

void ATG_Gun::BeginPlay()
{
	Super::BeginPlay();
	// InitAnimation();
}

void ATG_Gun::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATG_Gun::PullTrigger()
{
	if (!WeaponOwner || WeaponOwner->GetIsDead()) return;
	
	GetWorldTimerManager().SetTimer(FireTimer, this, &ThisClass::OnFire, TimeBetweenShoots, true);
	OnFire();
	// OnFiring.Broadcast(true);
}

void ATG_Gun::ReleaseTrigger()
{
	GetWorldTimerManager().ClearTimer(FireTimer);
	OnFiring.Broadcast(false);
}

void ATG_Gun::SetAnimInstance(UAnimInstance* NewAnimInstance)
{
	AnimInstance = NewAnimInstance;
}

void ATG_Gun::OnFire()
{
	/* Restart fire animation after each shot */
	OnFiring.Broadcast(false);
	OnFiring.Broadcast(true);

	if (ProjectileClass)
	{
		const FRotator SpawnRotator = FP_Muzzle->GetComponentRotation();
		const FVector  SpawnLocation = FP_Muzzle->GetComponentLocation();

		ATG_Projectile* Projectile = GetWorld()->SpawnActor<ATG_Projectile>(ProjectileClass, SpawnLocation, SpawnRotator);
		if (!Projectile) return;
		
		Projectile->SetProjectileOwner(this);
	}
	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}
	// UAISense_Hearing::ReportNoiseEvent(GetWorld(), WeaponOwner->GetActorLocation());
}

void ATG_Gun::SetWeaponOwner(ATG_BaseCharacter* TheOwner)
{
	WeaponOwner = TheOwner;
}

void ATG_Gun::InitAnimation()
{
	for (const auto Animation : OwnerFireAnimations)
	{
		if (!Animation) continue;

		const TArray<FAnimNotifyEvent> NotifyEvents = Animation->Notifies;
		for (auto NotifyEvent : NotifyEvents)
		{
			const auto AnimNotify = Cast<UTG_AN_Fire>(NotifyEvent.Notify);
			if (!AnimNotify) continue;

			AnimNotify->OnGunFired.AddUObject(this, &ThisClass::OnFireAnimStarted);
		}
	}
}

// ReSharper disable CppParameterMayBeConstPtrOrRef
void ATG_Gun::OnFireAnimStarted(USkeletalMeshComponent* MeshComp)
{
	if (!WeaponOwner || WeaponOwner->GetMesh() != MeshComp) return;

	OnFire();
}

float ATG_Gun::GetWeaponDamage() const
{
	return Damage;
}
