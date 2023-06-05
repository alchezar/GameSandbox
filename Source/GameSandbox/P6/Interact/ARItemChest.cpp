// Copyright (C) 2023, IKinder

#include "ARItemChest.h"
#include "Components/StaticMeshComponent.h"

AARItemChest::AARItemChest()
{
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	BaseMesh->SetupAttachment(RootComponent);
	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(BaseMesh);
}

void AARItemChest::BeginPlay()
{
	Super::BeginPlay();
}

void AARItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AARItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	LidMesh->SetRelativeRotation(FRotator(TargetPitch, 0.f, 0.f));
}
