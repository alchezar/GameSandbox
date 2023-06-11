// Copyright (C) 2023, IKinder

#include "ARPowerupHealthPotion.h"
#include "P6/Component/ARAttributesComponent.h"

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
	if (!AttributesComponent) return;

	if (AttributesComponent->TryChangeHealth(this, AttributesComponent->GetHealthMax()))
	{
		CooldownPowerup();
	}
}

