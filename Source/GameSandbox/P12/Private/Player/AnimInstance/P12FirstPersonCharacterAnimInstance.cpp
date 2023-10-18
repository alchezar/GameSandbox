// Copyright (C) 2023, IKinder

#include "P12/Public/Player/AnimInstance/P12FirstPersonCharacterAnimInstance.h"

#include "P12/Public/Player/P12BaseCharacter.h"
#include "P12/Public/Player/Controller/P12PlayerController.h"

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
