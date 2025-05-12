// Copyright © 2025, Ivan Kinder

#include "Player/P16PlayerCheckpoint.h"

#include "Project16.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Game/P16GameMode.h"
#include "Interface/P16PlayerInterface.h"

class AP16GameMode;

AP16PlayerCheckpoint::AP16PlayerCheckpoint(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("CheckpointStaticMeshComponent");
	Mesh->SetupAttachment(GetRootComponent());
	Mesh->SetCustomDepthStencilValue(P16::CustomDepthBlue);

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

void AP16PlayerCheckpoint::LoadActor_Implementation()
{
	if (bReached)
	{
		HandleGlowEffects();
	}
}

void AP16PlayerCheckpoint::ToggleHighlight_Implementation(const bool bOn)
{
	Mesh->SetRenderCustomDepth(bOn);
}

void AP16PlayerCheckpoint::UpdateDestination_Implementation(FVector& OutDestination)
{
	OutDestination = GetCapsuleComponent()->GetComponentLocation();
}

void AP16PlayerCheckpoint::OnSphereBeginOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	EARLY_RETURN_IF(!OtherActor->Implements<UP16PlayerInterface>())
	HandleGlowEffects();

	// Save in game progress.
	bReached = true;

	if (const AP16GameMode* GameMode = GetWorld()->GetAuthGameMode<AP16GameMode>())
	{
		GameMode->SaveWorldState(GetWorld());
	}

	IP16PlayerInterface::Execute_SaveProgress(OtherActor, PlayerStartTag);
}

void AP16PlayerCheckpoint::HandleGlowEffects()
{
	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	OnCheckpointReached(Mesh->CreateAndSetMaterialInstanceDynamic(0));
}
