// Copyright (C) 2023, IKinder

#include "Player/AnimInstance/P12FirstPersonCharacterAnimInstance.h"

#include "Player/P12BaseCharacter.h"
#include "Player/Controller/P12PlayerController.h"

void UP12FirstPersonCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void UP12FirstPersonCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!GetCachedCharacter().IsValid())
	{
		return;
	}

	PlayerCameraPitchAngle = CalculateCameraPitchAngle();
}

float UP12FirstPersonCharacterAnimInstance::CalculateCameraPitchAngle() const
{
	const AP12PlayerController* Controller = GetCachedCharacter()->GetController<AP12PlayerController>();
	if (!Controller || Controller->GetIsIgnoreCameraPitch())
	{
		return 0.f;
	}
	return 0.f - Controller->GetControlRotation().Pitch;
}
