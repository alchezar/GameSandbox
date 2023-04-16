// Copyright (C) 2023, IKinder

#include "STU_AICharacter.h"

#include "STU_AIController.h"
#include "GameFramework/CharacterMovementComponent.h"

ASTU_AICharacter::ASTU_AICharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI     = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = ASTU_AIController::StaticClass();

	bUseControllerRotationYaw = false;
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->RotationRate = FRotator(0.0, 200.0, 0.0);
	}
}

void ASTU_AICharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ASTU_AICharacter::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASTU_AICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
