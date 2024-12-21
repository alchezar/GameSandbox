// Copyright (C) 2023, IKinder

#include "Interact/ARPowerupBase.h"

#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"

AARPowerupBase::AARPowerupBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Overlap);
	SetRootComponent(SphereComp);

	// SetReplicates(true);
	bReplicates = true;
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

void AARPowerupBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, bActive);
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

void AARPowerupBase::OnRep_Active()
{
	SetActorEnableCollision(bActive);
	RootComponent->SetVisibility(bActive, true);
}

void AARPowerupBase::SetPowerupState(bool bNewState)
{
	SetActorEnableCollision(bNewState);
	RootComponent->SetVisibility(bNewState, true);
}

