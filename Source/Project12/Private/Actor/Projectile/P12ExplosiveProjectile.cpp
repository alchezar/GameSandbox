// Copyright (C) 2023, IKinder

#include "Actor/Projectile/P12ExplosiveProjectile.h"

#include "Component/Scene/P12ExplosionComponent.h"
#include "Components/SphereComponent.h"

AP12ExplosiveProjectile::AP12ExplosiveProjectile()
{
	ExplosionComponent = CreateDefaultSubobject<UP12ExplosionComponent>("ExplosionComponent");
}

void AP12ExplosiveProjectile::LaunchProjectile(const FVector& Direction, AActor* LaunchedFrom)
{
	Super::LaunchProjectile(Direction, LaunchedFrom);

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
