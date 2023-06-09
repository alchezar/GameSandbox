// Copyright (C) 2023, IKinder

#include "ARAnimInstance.h"

#include "ARCharacter.h"
#include "P6/Component/ARAttributesComponent.h"

void UARAnimInstance::NativeInitializeAnimation()
{
	Pawn = TryGetPawnOwner();
	if (!Pawn) return;

	MovementComponent = Pawn->GetMovementComponent();
	AttributesComponent = Pawn->FindComponentByClass<UARAttributesComponent>();
}

void UARAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	bAlive = AttributesComponent ? AttributesComponent->GetIsAlive() : false;
	
	// if (AARCharacter* Character = Cast<AARCharacter>(Pawn))
	// {}
}
