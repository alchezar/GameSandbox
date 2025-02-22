// Copyright (C) 2023, IKinder

#include "Weapon/Projectile/STU_ProjectileBullet.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Player/STU_PlayerState.h"
#include "Weapon/STU_BlasterWeapon.h"
#include "Weapon/Component/STU_WeaponFXComponent.h"

ASTU_ProjectileBullet::ASTU_ProjectileBullet()
{
	PrimaryActorTick.bCanEverTick = true;

	MovementComponent->ProjectileGravityScale = GravityScale;
}

void ASTU_ProjectileBullet::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();
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

	FPointDamageEvent DamageEvent;
	DamageEvent.HitInfo = HitResult;
	HitActor->TakeDamage(DamageAmount, DamageEvent, GetController(), this);
	WeaponFXComponent->PlayImpactFX(HitResult);
	Destroy();
}

void ASTU_ProjectileBullet::SpawnTraceFX()
{
	const ASTU_BlasterWeapon* BlasterWeapon = Cast<ASTU_BlasterWeapon>(GetOwner());
	if (!BlasterWeapon) return;

	TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
		TraceFX,
		CollisionComponent,
		NAME_None,
		FVector::ZeroVector,
		BlasterWeapon->GetWeaponSocketRotation(),
		EAttachLocation::SnapToTarget,
		false);

	SetBlasterColor();
	/* Random cone shooting direction from blaster weapon for correct bolt tale location */
	ForwardVector = BlasterWeapon->GetShootDirection().GetSafeNormal();
	/* Debug Blaster bolt tale location for first frame */
	UpdateBoltTaleOffset();
}

void ASTU_ProjectileBullet::UpdateBoltTaleOffset(FVector TaleOffset) const
{
	if (!TraceFXComponent) return;

	/* Avoid spawning bolt with full tale length at beginning */
	const bool bFarEnough = FVector::Dist(StartLocation, GetActorLocation()) >= BlasterBoltLength;
	TaleOffset            = bFarEnough ? TaleOffset : GetActorLocation() - StartLocation;

	TraceFXComponent->SetVariableVec3(TraceTargetName, GetActorLocation() - TaleOffset);
}

void ASTU_ProjectileBullet::SetBlasterColor()
{
	const ASTU_BlasterWeapon* BlasterWeapon = Cast<ASTU_BlasterWeapon>(GetOwner());
	if (!BlasterWeapon) return;

	const APawn* Pawn = Cast<APawn>(BlasterWeapon->GetOwner());
	if (!Pawn) return;

	const AController* Controller = Pawn->GetController();
	if (!Controller) return;

	const ASTU_PlayerState* PlayerState = Cast<ASTU_PlayerState>(Controller->PlayerState);
	if (!PlayerState)
	{
		TraceFXComponent->SetVariableLinearColor(BlasterColorName, FLinearColor(10.f, 0.f, 0.f));
		return;
	}

	const FLinearColor BlasterColor = PlayerState->GetBlasterColor();

	TraceFXComponent->SetVariableLinearColor(BlasterColorName, BlasterColor);
}
