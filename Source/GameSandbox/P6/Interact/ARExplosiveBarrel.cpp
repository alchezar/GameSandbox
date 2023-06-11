// Copyright (C) 2023, IKinder

#include "ARExplosiveBarrel.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "P6/Component/ARAttributesComponent.h"

AARExplosiveBarrel::AARExplosiveBarrel()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	SetRootComponent(MeshComp);
	MeshComp->SetSimulatePhysics(true);
	
	ForceComp = CreateDefaultSubobject<URadialForceComponent>("ForceComponent");
	ForceComp->SetupAttachment(RootComponent);
	ForceComp->SetAutoActivate(false);
	ForceComp->Radius = Radius;
	ForceComp->ImpulseStrength = Impulse;
	ForceComp->bImpulseVelChange = true;
	ForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);
}

void AARExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
}

void AARExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AARExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	MeshComp->OnComponentHit.AddDynamic(this, &ThisClass::OnBarrelHit);
}

void AARExplosiveBarrel::OnBarrelHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ForceComp->FireImpulse();

	UARAttributesComponent* AttributesComponent = UARAttributesComponent::GetAttributes(OtherActor);
	if (!AttributesComponent) return;

	AttributesComponent->TryChangeHealth(this, -Damage);
}

