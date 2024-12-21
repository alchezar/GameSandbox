// Copyright (C) 2023, IKinder

#include "Interact/ARBonfire.h"

#include "Game/ARGameModeBase.h"
#include "Particles/ParticleSystemComponent.h"

AARBonfire::AARBonfire()
{
	Firewood = CreateDefaultSubobject<UStaticMeshComponent>("Firewood");
	Firewood->SetupAttachment(RootComponent);

	FireEffect = CreateDefaultSubobject<UParticleSystemComponent>("FireEffect");
	FireEffect->SetupAttachment(Firewood);
	FireEffect->bAutoActivate = false;
}

void AARBonfire::BeginPlay()
{
	Super::BeginPlay();
	check(FireEffect);
}

void AARBonfire::UpdateFlame()
{
	bActiveFire ? FireEffect->Activate() : FireEffect->Deactivate();
}

void AARBonfire::Interact_Implementation(APawn* InstigatorPawn)
{
	bActiveFire = true;
	AARGameModeBase* GameMode = Cast<AARGameModeBase>(GetWorld()->GetAuthGameMode());
	if (!GameMode) return;
	
	GameMode->WriteSaveGame();
	UpdateFlame();
}

void AARBonfire::OnActorLoaded_Implementation()
{
	IARGameplayInterface::OnActorLoaded_Implementation();
	UpdateFlame();
}