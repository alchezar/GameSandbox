// Copyright (C) 2023, IKinder

#include "Weapon/Projectile/STU_ProjectileRocket.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon/Component/STU_WeaponFXComponent.h"

ASTU_ProjectileRocket::ASTU_ProjectileRocket()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASTU_ProjectileRocket::BeginPlay()
{
	Super::BeginPlay();
}

void ASTU_ProjectileRocket::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASTU_ProjectileRocket::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult)
{
	if (!GetWorld()) return;

	MovementComponent->StopMovementImmediately();
	UGameplayStatics::ApplyRadialDamage(
		GetWorld(),
		DamageAmount,
		GetActorLocation(),
		DamageRadius,
		UDamageType::StaticClass(),
		{/*GetOwner()*/},
		this,
		GetController(),
		bFullDamage);
	WeaponFXComponent->PlayImpactFX(HitResult);

	Destroy();
}
