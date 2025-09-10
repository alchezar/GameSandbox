// Copyright © 2025, Ivan Kinder

#include "Item/Pickup/P17PickupBase.h"

#include "Components/SphereComponent.h"

AP17PickupBase::AP17PickupBase()
{
	Collision = CreateDefaultSubobject<USphereComponent>("CollisionSphereComponent");
	SetRootComponent(Collision);
	Collision->InitSphereRadius(50.f);
}

void AP17PickupBase::BeginPlay()
{
	Super::BeginPlay();

	Collision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlapCallback);
}

void AP17PickupBase::OnBeginOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{}
