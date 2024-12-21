// Copyright (C) 2023, IKinder

#include "Weapon/ARProjectileHole.h"

#include "PhysicsEngine/RadialForceComponent.h"

AARProjectileHole::AARProjectileHole()
{
	PrimaryActorTick.bCanEverTick = true;

	ForceComp = CreateDefaultSubobject<URadialForceComponent>("ForceComponent");
	ForceComp->SetupAttachment(RootComponent);
	ForceComp->SetAutoActivate(false);
	ForceComp->Radius = 1000.f;
	ForceComp->ImpulseStrength = 1000.f;
	ForceComp->ForceStrength = - 2000000.f;
	ForceComp->bImpulseVelChange = true;
	ForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);
}

void AARProjectileHole::BeginPlay()
{
	Super::BeginPlay();
}

void AARProjectileHole::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ForceComp->ImpulseStrength = 0.f;
}

void AARProjectileHole::OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnProjectileBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AARProjectileHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

