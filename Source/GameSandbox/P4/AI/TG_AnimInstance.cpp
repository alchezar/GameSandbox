// Copyright (C) 2023, IKinder

#include "TG_AnimInstance.h"
#include "GameFramework/PawnMovementComponent.h"
#include "P4/Player/TG_FirstPersonCharacter.h"
#include "P4/Weapon/TG_Gun.h"

void UTG_AnimInstance::NativeInitializeAnimation()
{
	Pawn = TryGetPawnOwner();
	if (!Pawn) return;

	MovementComponent = Pawn->GetMovementComponent();	
}

void UTG_AnimInstance::NativeUpdateAnimation(const float DeltaSeconds)
{
	if (!Pawn || !MovementComponent) return;

	bInAir = MovementComponent->IsFalling();
	Speed = Pawn->GetVelocity().Size();
	Direction = GetMovementDirectionAngle();
	AimRotation = (Pawn->GetBaseAimRotation() - Pawn->GetActorRotation());
	AimRotation.Normalize();	
	
	if (const auto* PersonCharacter = Cast<ATG_FirstPersonCharacter>(Pawn))
	{
		ATG_Gun* Gun = PersonCharacter->GetCurrentWeapon();
		if (!Gun) return;
		
		Gun->OnFiring.AddUObject(this, &ThisClass::OnFiringHandle);
	}
}

float UTG_AnimInstance::GetMovementDirectionAngle() const
{
	if (Pawn->GetVelocity().IsZero()) return 0.f;

	const FVector VelocityNormal = Pawn->GetVelocity().GetSafeNormal();
	const double  AngleBetween = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(Pawn->GetActorForwardVector(), VelocityNormal)));
	const FVector CrossProduct = FVector::CrossProduct(Pawn->GetActorForwardVector(), VelocityNormal);

	return AngleBetween * FMath::Sign(CrossProduct.Z);
}

void UTG_AnimInstance::OnFiringHandle(const bool bFire)
{
	bShooting = bFire;
}
