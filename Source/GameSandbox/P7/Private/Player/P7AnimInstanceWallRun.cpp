// Copyright (C) 2023, IKinder

#include "P7/Public/Player/P7AnimInstanceWallRun.h"
#include "P7/Public/Component/P7WallRunComponent.h"
#include "P7/Public/Player/P7Character.h"

void UP7AnimInstanceWallRun::NativeInitializeAnimation()
{
	Player = Cast<AP7Character>(TryGetPawnOwner());
	if (!Player) return;
	WallRunComponent = Player->GetWallRunComponent();
}

void UP7AnimInstanceWallRun::NativeUpdateAnimation(float DeltaSeconds)
{
	if (!Player || !WallRunComponent) return;
	bWallRun  = WallRunComponent->GetIsWallRunning();
	bWallJump = WallRunComponent->GetIsWallJumping();
	WallSide  = WallRunComponent->GetWallSide();
	CurrentArcAngle = WallRunComponent->GetArcCurrAngle();
	TurnRate = WallRunComponent->GetTurnRate();
	CurrentSpeed = Player->GetVelocity().Length();
	/** The speed of the wall jump animation is increased relative to the desired speed of the wall jump.
	  * This improves the look of higher speed wall jumps because the animation will better match the faster movement.
	  * The bottom value is 943 because that is the vector length of the default wall jump velocity. */
	const FVector WallJumpVelocity = FVector(0.f, WallRunComponent->GetJumpHorizontalVelocity(), WallRunComponent->GetJumpVerticalVelocity());
	WallJumpAnimSpeed = FMath::GetMappedRangeValueClamped(FVector2D(940.f, 1500.f), FVector2D(1.f, 1.5f), WallJumpVelocity.Length());
	/** Set how fast the run animation plays according to how fast the character is moving. 800 movement speed = default animation speed.
	  * The animation can only go +/- 15% speed to prevent it from looking ridiculous at very high or very low movement speeds. */
	CurrentAnimPlayRate = FMath::GetMappedRangeValueClamped(FVector2D(680.f, 920.f), FVector2D(0.85, 1.15f), CurrentSpeed);
	/** Calculate an alpha value (0-1) that will be used for the hand correction additive pose.
	  * This additive pose is an aid to help the hand position remain consistent during the arcing wall run blendspace.
	  * Without this, the hand will clip into the wall between 0 and 45 degrees in the blendspace. */
	float ArcAngleAlpha;
	if (CurrentArcAngle >= 15.f)
	{
		ArcAngleAlpha = FMath::GetMappedRangeValueClamped(FVector2D(15.f, 45.f), FVector2D(1.f, 0.f), CurrentArcAngle);
	}
	else
	{
		ArcAngleAlpha = FMath::GetMappedRangeValueClamped(FVector2D(0.f, 15.f), FVector2D(0.f, 1.f), CurrentArcAngle);
	}
	HandCorrectionAdditive = FMath::FInterpTo(HandCorrectionAdditive, ArcAngleAlpha, GetWorld()->GetDeltaSeconds(), 7.f);
}
