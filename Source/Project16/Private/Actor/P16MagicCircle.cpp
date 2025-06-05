// Copyright © 2025, Ivan Kinder

#include "Actor/P16MagicCircle.h"

#include "Components/DecalComponent.h"

AP16MagicCircle::AP16MagicCircle()
{
	PrimaryActorTick.bCanEverTick = true;

	MagicCircleDecal = CreateDefaultSubobject<UDecalComponent>("MagicCircleDecalComponent");
	MagicCircleDecal->SetupAttachment(GetRootComponent());
}

void AP16MagicCircle::BeginPlay()
{
	Super::BeginPlay();
}

void AP16MagicCircle::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	const FRotator Delta = FRotator {0.f, 0.f, DeltaTime * RotationSpeed};
	MagicCircleDecal->AddLocalRotation(Delta);
}

void AP16MagicCircle::SetDecalMaterial(UMaterialInterface* DecalMaterial) const
{
	MagicCircleDecal->SetMaterial(0, DecalMaterial);
}
