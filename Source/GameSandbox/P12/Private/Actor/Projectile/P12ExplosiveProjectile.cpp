// Copyright (C) 2023, IKinder

#include "P12/Public/Actor/Projectile/P12ExplosiveProjectile.h"

#include "Components/SphereComponent.h"
#include "P12/Public/Component/Scene/P12ExplosionComponent.h"

AP12ExplosiveProjectile::AP12ExplosiveProjectile()
{
	ExplosionComponent = CreateDefaultSubobject<UP12ExplosionComponent>("ExplosionComponent");
	// ExplosionComponent->SetupAttachment(RootComponent);
}

void AP12ExplosiveProjectile::LaunchProjectile(const FVector& Direction, AP12ThrowableItem* ThrowableItem)
{
	Super::LaunchProjectile(Direction, ThrowableItem);

	ExplosionComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	GetWorld()->GetTimerManager().SetTimer(DetonationHandle, this, &ThisClass::Detonate, DetonationTime);
}

void AP12ExplosiveProjectile::Detonate()
{
	AController* Controller = nullptr;
	if (GetOwner()->IsA<APawn>())
	{
		const APawn* Char = StaticCast<APawn*>(GetOwner());
		Controller = Char->Controller;
	}
	ExplosionComponent->Explode(Controller);
	
	MeshComponent->SetVisibility(false);
	CollisionComponent->SetSimulatePhysics(false);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetLifeSpan(5.f);
}

