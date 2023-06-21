// Copyright (C) 2023, IKinder

#include "ARItemChest.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

AARItemChest::AARItemChest()
{
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	BaseMesh->SetupAttachment(RootComponent);
	BaseMesh->SetSimulatePhysics(true);
	BaseMesh->SetIsReplicated(true);
	
	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(BaseMesh);

	SetReplicates(true);
	SetReplicatingMovement(true);
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
	bLidOpen = !bLidOpen;
	OnRep_LidOpened();
}

void AARItemChest::OnActorLoaded_Implementation()
{
	OnRep_LidOpened();	
}

void AARItemChest::OnRep_LidOpened()
{
	const float CurrentPitch = bLidOpen ? TargetPitch : 0.f;
	LidMesh->SetRelativeRotation(FRotator(CurrentPitch, 0.f, 0.f));
}

void AARItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, bLidOpen);
}
