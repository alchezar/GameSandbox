// Copyright (C) 2023, IKinder

#include "P12/Public/Component/P12BaseCharacterMovementComponent.h"

UP12BaseCharacterMovementComponent::UP12BaseCharacterMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UP12BaseCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UP12BaseCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UP12BaseCharacterMovementComponent::DefaultSetup()
{
	NavAgentProps.bCanCrouch = true;
	SetPawnRotationMode(true);
}

void UP12BaseCharacterMovementComponent::SetPawnRotationMode(const bool bOrientToMovement)
{
	if (bOrientRotationToMovement == bOrientToMovement)
	{
		return;
	}
	bUseControllerDesiredRotation = !bOrientToMovement;
	bOrientRotationToMovement = bOrientToMovement;
}

void UP12BaseCharacterMovementComponent::ToggleMaxSpeed(const bool bRun)
{
	MaxWalkSpeed = bRun ? MaxSpeed.Run : MaxSpeed.Walk;
}

