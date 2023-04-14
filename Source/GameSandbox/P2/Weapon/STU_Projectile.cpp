// Copyright (C) 2023, IKinder

#include "STU_Projectile.h"
#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameSandbox/P2/Weapon/Component/STU_WeaponFXComponent.h"
#include "Kismet/GameplayStatics.h"

ASTU_Projectile::ASTU_Projectile()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	CollisionComponent->InitSphereRadius(5.f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Block);
	SetRootComponent(CollisionComponent);

	MovementComponent               = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
	MovementComponent->InitialSpeed = 2000.f;

	WeaponFXComponent = CreateDefaultSubobject<USTU_WeaponFXComponent>("WeaponFXComponent");
}

void ASTU_Projectile::BeginPlay()
{
	Super::BeginPlay();
	check(CollisionComponent);
	check(MovementComponent);
	check(WeaponFXComponent);

	MovementComponent->Velocity = ShotDirection * MovementComponent->InitialSpeed;
	CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
	CollisionComponent->OnComponentHit.AddDynamic(this, &ASTU_Projectile::OnProjectileHit);
	CollisionComponent->bReturnMaterialOnMove = true;
	SetLifeSpan(LifeTime);
}

void ASTU_Projectile::SetShotDirection(const FVector& Direction)
{
	ShotDirection = Direction;
}

void ASTU_Projectile::OnProjectileHit(
	UPrimitiveComponent* HitComponent,
	AActor*              OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector              NormalImpulse,
	const FHitResult&    Hit)
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
	// DrawDebugSphere(GetWorld(), GetActorLocation(), DamageRadius, 32, FColor::Red, false, LifeTime);
	WeaponFXComponent->PlayImpactFX(Hit);

	Destroy();
}

AController* ASTU_Projectile::GetController() const
{
	const auto Pawn = Cast<APawn>(GetOwner());
	return Pawn ? Pawn->GetController() : nullptr;
}

UProjectileMovementComponent* ASTU_Projectile::GetMovementComponent() const
{
	return MovementComponent;
}

float ASTU_Projectile::GetLifeTime() const
{
	return LifeTime;
}
