// Copyright (C) 2023, IKinder

#include "Interact/ARPowerupHealthPotion.h"

#include "Component/ARAttributesComponent.h"
#include "Player/ARPlayerState.h"

AARPowerupHealthPotion::AARPowerupHealthPotion()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);
}

void AARPowerupHealthPotion::BeginPlay()
{
	Super::BeginPlay();
}

void AARPowerupHealthPotion::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AARPowerupHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!InstigatorPawn) return;

	UARAttributesComponent* AttributesComponent = UARAttributesComponent::GetAttributes(InstigatorPawn);
	if (!AttributesComponent || AttributesComponent->GetIsHealthMax()) return;

	AARPlayerState* PlayerState = InstigatorPawn->GetPlayerState<AARPlayerState>();
	if (!PlayerState) return;
	
	if (!PlayerState->RemoveCredits(CreditsCost)) return;
	if (!AttributesComponent->TryChangeHealth(this, AttributesComponent->GetHealthMax())) return;

	CooldownPowerup();
}
