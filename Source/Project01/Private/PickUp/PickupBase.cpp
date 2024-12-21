// Copyright (C) 2023, IKinder

#include "Project01/Public/PickUp/PickupBase.h"

APickUpBase::APickUpBase()
{
	PrimaryActorTick.bCanEverTick = true;

	PickupMesh    = CreateDefaultSubobject<UStaticMeshComponent>("Pickup Mesh");
	RootComponent = PickupMesh;
}

void APickUpBase::BeginPlay()
{
	Super::BeginPlay();
}

void APickUpBase::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool APickUpBase::GetPickUpActive() const
{
	return bActive;
}

void APickUpBase::SetPickUpActive(const bool NewActive)
{
	bActive = NewActive;
}

void APickUpBase::OnPickupCollect_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("%s: Picked up"), *GetName()));
	
}
