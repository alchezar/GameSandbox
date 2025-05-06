// Copyright © 2025, Ivan Kinder

#include "Player/P16PlayerCheckpoint.h"

#include "Project16.h"
#include "Components/SphereComponent.h"
#include "Interface/P16PlayerInterface.h"

AP16PlayerCheckpoint::AP16PlayerCheckpoint(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("CheckpointStaticMeshComponent");
	Mesh->SetupAttachment(GetRootComponent());

	Sphere = CreateDefaultSubobject<USphereComponent>("SphereOverlapComponent");
	Sphere->SetupAttachment(GetRootComponent());
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AP16PlayerCheckpoint::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereBeginOverlapCallback);
}

void AP16PlayerCheckpoint::OnSphereBeginOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	EARLY_RETURN_IF(!OtherActor->ActorHasTag("Player"))

	// Handle glow effect.
	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	OnCheckpointReached(Mesh->CreateAndSetMaterialInstanceDynamic(0));

	// Save in game progress.
	IP16PlayerInterface::Execute_SaveProgress(OtherActor, PlayerStartTag);
}
