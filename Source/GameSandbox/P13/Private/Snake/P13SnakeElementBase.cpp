// Copyright © 2024, IKinder

#include "P13/Public/Snake/P13SnakeElementBase.h"

AP13SnakeElementBase::AP13SnakeElementBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SnakeElementMesh = CreateDefaultSubobject<UStaticMeshComponent>("SnakeElementStaticMeshComponent");
	SnakeElementMesh->SetupAttachment(GetRootComponent());
}

void AP13SnakeElementBase::BeginPlay()
{
	Super::BeginPlay();
}

void AP13SnakeElementBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
