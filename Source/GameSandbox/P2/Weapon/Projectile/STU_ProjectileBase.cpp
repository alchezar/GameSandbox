// Copyright (C) 2023, IKinder

#include "STU_ProjectileBase.h"
#include "DrawDebugHelpers.h"
#include "Weapon/STU_BaseWeapon.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameSandbox/P2/Weapon/Component/STU_WeaponFXComponent.h"

ASTU_ProjectileBase::ASTU_ProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	CollisionComponent->InitSphereRadius(ProjectileRadius);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Block);
	SetRootComponent(CollisionComponent);

	MovementComponent               = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
	MovementComponent->InitialSpeed = ProjectileInitialSpeed;

	WeaponFXComponent = CreateDefaultSubobject<USTU_WeaponFXComponent>("WeaponFXComponent");
}

void ASTU_ProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	check(CollisionComponent);
	check(MovementComponent);
	check(WeaponFXComponent);

	MovementComponent->Velocity = ShotDirection * ProjectileInitialSpeed;
	CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
	CollisionComponent->OnComponentHit.AddDynamic(this, &ASTU_ProjectileBase::OnProjectileHit);
	CollisionComponent->bReturnMaterialOnMove = true;
	SetLifeSpan(LifeTime);
}

void ASTU_ProjectileBase::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult)
{}

UProjectileMovementComponent* ASTU_ProjectileBase::GetMovementComponent()
{
	return MovementComponent;
}

float ASTU_ProjectileBase::GetLifeTime() const
{
	return LifeTime;
}

void ASTU_ProjectileBase::SetShotDirection(const FVector& Direction)
{
	ShotDirection = Direction;
}

AController* ASTU_ProjectileBase::GetController() const
{
	// const auto Pawn = Cast<APawn>(GetOwner());
	// return Pawn ? Pawn->GetController() : nullptr;

	const ASTU_BaseWeapon* BaseWeapon = Cast<ASTU_BaseWeapon>(GetOwner());
	if (!BaseWeapon) return nullptr;

	const APawn* Pawn = Cast<APawn>(BaseWeapon->GetOwner());
	if (!Pawn) return nullptr;

	return Pawn->GetController();
}

