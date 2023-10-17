// Copyright (C) 2023, IKinder

#include "P12/Public/Player/AnimInstance/P12FirstPersonCharacterAnimInstance.h"

#include "P12/Public/Player/P12BaseCharacter.h"

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
	const APlayerController* Controller = GetCachedCharacter()->GetController<APlayerController>();
	if (!Controller)
	{
		return;
	}
	PlayerCameraPitchAngle = 0.f - Controller->GetControlRotation().Pitch;
}
