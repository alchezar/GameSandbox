// Copyright (C) 2023, IKinder

#include "AI/TG_AI_Character.h"

ATG_AI_Character::ATG_AI_Character()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATG_AI_Character::BeginPlay()
{
	Super::BeginPlay();
}

void ATG_AI_Character::CharacterDying()
{
	Super::CharacterDying();
	GetController()->UnPossess();	
}

void ATG_AI_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

