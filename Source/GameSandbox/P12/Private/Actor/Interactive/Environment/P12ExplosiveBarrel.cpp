// Copyright (C) 2023, IKinder

#include "P12/Public/Actor/Interactive/Environment/P12ExplosiveBarrel.h"

#include "P12/Public/Component/Scene/P12ExplosionComponent.h"

AP12ExplosiveBarrel::AP12ExplosiveBarrel()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>("SceneRootComponent");

	BarrelMesh = CreateDefaultSubobject<UStaticMeshComponent>("BarrelStaticMeshComponent");
	BarrelMesh->SetupAttachment(RootComponent);

	ExplosionComponent = CreateDefaultSubobject<UP12ExplosionComponent>("BarrelExplosionComponent");
	ExplosionComponent->SetupAttachment(RootComponent);
}

void AP12ExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();

	OnTakeAnyDamage.AddDynamic(this, &ThisClass::OnTakeAnyDamageHandle);
	ExplosionComponent->OnExplosion.AddUObject(this, &ThisClass::OnExplosionHandle);
}

void AP12ExplosiveBarrel::OnTakeAnyDamageHandle(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	ExplosionComponent->Explode(InstigatedBy);
}

void AP12ExplosiveBarrel::OnExplosionHandle()
{
	BarrelMesh->SetVisibility(false);
	BarrelMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetLifeSpan(5.f);
}
