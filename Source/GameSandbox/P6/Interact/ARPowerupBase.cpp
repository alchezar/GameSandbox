// Copyright (C) 2023, IKinder

#include "ARPowerupBase.h"
#include "Components/SphereComponent.h"

AARPowerupBase::AARPowerupBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Overlap);
	SetRootComponent(SphereComp);

	SetReplicates(true);
}

void AARPowerupBase::BeginPlay()
{
	Super::BeginPlay();
}

void AARPowerupBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AARPowerupBase::Interact_Implementation(APawn* InstigatorPawn)
{
	IARGameplayInterface::Interact_Implementation(InstigatorPawn);
}

void AARPowerupBase::ShowPowerup()
{
	SetPowerupState(true);
}

void AARPowerupBase::CooldownPowerup()
{
	SetPowerupState(false);
	GetWorld()->GetTimerManager().ClearTimer(RespawnTimer);
	GetWorld()->GetTimerManager().SetTimer(RespawnTimer, this, &ThisClass::ShowPowerup, RespawnTime);
}

void AARPowerupBase::SetPowerupState(bool bNewState)
{
	SetActorEnableCollision(bNewState);
	RootComponent->SetVisibility(bNewState, true);
}

