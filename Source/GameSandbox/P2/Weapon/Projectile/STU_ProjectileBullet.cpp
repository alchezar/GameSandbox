// Copyright (C) 2023, IKinder

#include "STU_ProjectileBullet.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "STU_BlasterWeapon.h"
#include "Components/SphereComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameSandbox/P2/Weapon/Component/STU_WeaponFXComponent.h"
#include "Kismet/GameplayStatics.h"

ASTU_ProjectileBullet::ASTU_ProjectileBullet()
{
	PrimaryActorTick.bCanEverTick = true;

	MovementComponent->ProjectileGravityScale = GravityScale;
}

void ASTU_ProjectileBullet::BeginPlay()
{
	Super::BeginPlay();

	SpawnTraceFX();
}

void ASTU_ProjectileBullet::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateBoltTaleOffset(ForwardVector * BlasterBoltLength);
}

void ASTU_ProjectileBullet::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult)
{
	AActor* HitActor = HitResult.GetActor();
	if (!HitActor) return;

	//TODO: head bone name hardcode isn`t great idea 😅 it will be better to make an array
	const bool  bHeadshot     = HitResult.BoneName == FName("head");
	const float CurrentDamage = bHeadshot ? DamageAmount * HeadshotMultiplier : DamageAmount;

	HitActor->TakeDamage(CurrentDamage, FDamageEvent(), GetController(), this);
	WeaponFXComponent->PlayImpactFX(HitResult);
	Destroy();
}

void ASTU_ProjectileBullet::SpawnTraceFX()
{
	FVector  PlayerLocation;
	FRotator PlayerRotation;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPlayerViewPoint(OUT PlayerLocation, OUT PlayerRotation);
	// GetOwner <Weapon class> -> GetOwner <ACharacter*> -> PossessedBy -> AController*

	const ASTU_BlasterWeapon* BlasterWeapon = Cast<ASTU_BlasterWeapon>(GetOwner());
	if (BlasterWeapon)
	{
		ForwardVector = BlasterWeapon->GetShootDirection().GetSafeNormal();
	}

	TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
		TraceFX,
		CollisionComponent,
		NAME_None,
		FVector::ZeroVector,
		PlayerRotation,
		EAttachLocation::SnapToTarget,
		false);

	// Debug Blaster bolt tale location for first frame
	UpdateBoltTaleOffset();
}

void ASTU_ProjectileBullet::UpdateBoltTaleOffset(const FVector& TaleOffset) const
{
	if (TraceFXComponent)
	{
		TraceFXComponent->SetVariableVec3(TraceTargetName, GetActorLocation() + TaleOffset);
	}
}
