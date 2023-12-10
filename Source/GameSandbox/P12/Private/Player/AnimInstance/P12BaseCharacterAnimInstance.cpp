// Copyright (C) 2023, IKinder

#include "P12/Public/Player/AnimInstance/P12BaseCharacterAnimInstance.h"

#include "KismetAnimationLibrary.h"
#include "P12/Public/Actor/Equipment/Weapon/P12RangeWeaponItem.h"
#include "P12/Public/Component/Actor/P12EquipmentComponent.h"
#include "P12/Public/Component/Movement/P12BaseCharacterMovementComponent.h"
#include "P12/Public/Player/P12BaseCharacter.h"

void UP12BaseCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	CachedCharacter = Cast<AP12BaseCharacter>(TryGetPawnOwner());
	check(CachedCharacter.IsValid())
	CachedCharacterMovement = CachedCharacter->GetBaseCharacterMovement();
	check(CachedCharacterMovement.IsValid())
	CachedEquipmentComponent = CachedCharacter->GetEquipmentComponent();
	check(CachedEquipmentComponent.IsValid())

	CachedCharacter->OnLoadoutCreated.AddUObject(this, &ThisClass::OnLoadoutCreatedHandle);
}

void UP12BaseCharacterAnimInstance::OnLoadoutCreatedHandle(UP12EquipmentComponent* Equipment)
{
	CachedEquipmentComponent = Equipment;
	check(CachedEquipmentComponent.IsValid())
}

void UP12BaseCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!CachedCharacter.IsValid())
	{
		return;
	}
	Speed = CachedCharacter->GetVelocity().Size();
	// Direction = GetMovementDirectionAngle();
	Direction = UKismetAnimationLibrary::CalculateDirection(CachedCharacterMovement->Velocity, CachedCharacter->GetActorRotation());
	bInAir = CachedCharacterMovement->IsFalling();
	bCrouch = CachedCharacterMovement->IsCrouching();
	bLadder = CachedCharacterMovement->IsOnLadder();
	bAim = CachedCharacter->GetIsAiming();
	LadderSpeedRatio = CachedCharacterMovement->GetLadderSpeedRatio();
	ItemType = CachedEquipmentComponent->GetCurrentEquippedItemType();
	// AimRotation = (CachedCharacter->GetBaseAimRotation() - CachedCharacter->GetActorRotation()).GetNormalized();
	AimRotation = CachedCharacter->GetLocalAimOffset();

	LeftLegOffset = FVector(0.f, CachedCharacter->GetIKLeftLegOffset(), 0.f);
	RightLegOffset = FVector(0.f, CachedCharacter->GetIKRightLegOffset(), 0.f);
	HipOffset = FVector(0.f, CachedCharacter->GetIKHipOffset(), 0.f);

	if (const AP12RangeWeaponItem* CurrentWeapon = CachedEquipmentComponent->GetCurrentEquippedWeapon())
	{
		GripSocketTransform = CurrentWeapon->GetGripTransform();
	}
}

float UP12BaseCharacterAnimInstance::GetMovementDirectionAngle() const
{
	if (!CachedCharacter.IsValid() || FMath::IsNearlyZero(Speed))
	{
		return 0.f;
	}
	const FVector ForwardVector = CachedCharacter->GetActorForwardVector();
	const FVector VelocityVector = CachedCharacter->GetVelocity().GetSafeNormal2D();
	const float Angle = FMath::RadiansToDegrees(FMath::Acos(ForwardVector | VelocityVector));
	const float Sign = FMath::Sign((ForwardVector ^ VelocityVector).Z);
	return Sign * Angle;
}
