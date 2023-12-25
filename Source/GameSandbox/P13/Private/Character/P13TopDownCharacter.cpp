// Copyright © 2024, IKinder

#include "P13/Public/Character/P13TopDownCharacter.h"

#include "GameSandbox.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

AP13TopDownCharacter::AP13TopDownCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	// bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	
	CreateComponents();
}

void AP13TopDownCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AP13TopDownCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP13TopDownCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AP13TopDownCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	SetupCameraBoom();
}

void AP13TopDownCharacter::MoveInput(const FVector2D MoveVector)
{
	const FVector ForwardDirection = GetActorForwardVector();
	const FVector RightDirection = GetActorRightVector();
	
	AddMovementInput(ForwardDirection, MoveVector.Y);
	AddMovementInput(RightDirection, MoveVector.X);

	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (!PlayerController)
	{
		return;
	}
	
	FHitResult Hit;
	PlayerController->GetHitResultUnderCursor(ECC_CURSOR, true, Hit);
	if (!Hit.bBlockingHit)
	{
		return;
	}
	
	FVector LookAtDirection = (Hit.Location - GetActorLocation()).GetSafeNormal2D();
	RotateTowardMovement(LookAtDirection);
	// FRotator LookAtRotation = LookAtDirection.Rotation() - FRotator(0.f, 90.f, 0.f);
	// GetMesh()->SetWorldRotation(LookAtRotation);
}

void AP13TopDownCharacter::SprintInput(const bool bStart)
{
	/* Check if it can start sprinting. */
	if (MovementState < EP13MovementState::Walk)
	{
		return;
	}
	
	/* Cache the previous movement, to apply the correct one after the sprint is complete. */
	if (bStart)
	{
		PreviousMovementState = MovementState;
	}
	
	ChangeMovementState(bStart ? EP13MovementState::Sprint : PreviousMovementState);
}

void AP13TopDownCharacter::AimInput(const bool bStart)
{
	/* Check if it can start aiming. */
	if (MovementState > EP13MovementState::Run)
	{
		return;
	}

	/* Cache the previous movement, to apply the correct one after aiming. */
	if (bStart)
	{
		PreviousMovementState = MovementState;
	}
	
	ChangeMovementState(bStart ? EP13MovementState::Aim : PreviousMovementState);
}

void AP13TopDownCharacter::ZoomInput(const float Axis)
{
	ZoomSteps += Axis;
	float FinalLength = CameraBoom->TargetArmLength + ZoomSteps * ZoomStepDistance;
	FinalLength = FMath::Clamp(FinalLength, HeightClamp.Min, HeightClamp.Max);

	FTimerDelegate ScrollDelegate;
	ScrollDelegate.BindLambda([this]() { ZoomSteps = 0.f; });
	GetWorld()->GetTimerManager().SetTimer(ScrollTimer, ScrollDelegate, 1.f, false);
	
	FTimerDelegate ZoomDelegate;
	ZoomDelegate.BindUObject(this, &ThisClass::ZoomSmoothly, GetWorld()->GetDeltaSeconds(), FinalLength);
	GetWorld()->GetTimerManager().SetTimer(ZoomTimer, ZoomDelegate, GetWorld()->GetDeltaSeconds(), true);
}

void AP13TopDownCharacter::RotateTowardMovement(const FVector& LookAtDirection)
{
	const FRotator MeshOldRotation = GetMesh()->GetComponentRotation();
	const FRotator MeshNewRotation = LookAtDirection.Rotation() - FRotator(0.f, 90.f, 0.f);
	const FRotator InterpRotation = FMath::RInterpTo(MeshOldRotation, MeshNewRotation, GetWorld()->GetDeltaSeconds(), 2.f);
	GetMesh()->SetWorldRotation(InterpRotation);
}

void AP13TopDownCharacter::UpdateCharacterSpeed()
{
	float ResSpeed = GetCharacterMovement()->StaticClass()->GetDefaultObject<UCharacterMovementComponent>()->MaxWalkSpeed;
		
	if (MovementState == EP13MovementState::Aim)
	{
		ResSpeed = MovementSpeed.Aim;
	}
	else if (MovementState == EP13MovementState::Walk)
	{
		ResSpeed = MovementSpeed.Walk;
	}
	else if (MovementState == EP13MovementState::Run)
	{
		ResSpeed = MovementSpeed.Run;
	}
	else if (MovementState == EP13MovementState::Sprint)
	{
		ResSpeed = MovementSpeed.Sprint;
	}

	GetCharacterMovement()->MaxWalkSpeed = ResSpeed;
}

void AP13TopDownCharacter::ChangeMovementState(const EP13MovementState NewMovementState)
{
	MovementState = NewMovementState;
	UpdateCharacterSpeed();	
}

void AP13TopDownCharacter::CreateComponents()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoomSpringArmComponent");
	CameraBoom->SetupAttachment(RootComponent);
	
	TopDownCamera = CreateDefaultSubobject<UCameraComponent>("TopDownCameraComponent");
	TopDownCamera->SetupAttachment(CameraBoom);
}

void AP13TopDownCharacter::SetupCameraBoom() const
{
	CameraBoom->TargetArmLength = Height;
	CameraBoom->bEnableCameraLag = bFlow;
	CameraBoom->CameraLagSpeed = SmoothCoefficient;
	CameraBoom->SetRelativeRotation(CameraRotation);
}

void AP13TopDownCharacter::ShakeCamera() const
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController)
	{
		return;
	}
	PlayerController->ClientStartCameraShake(ShakeClass);
}

float AP13TopDownCharacter::GetIKSocketOffset(const FName& VirtualBoneName, const float TraceHalfDistance, const float FromBoneToBottom)
{
	const FVector SocketLocation = GetMesh()->GetBoneLocation(VirtualBoneName);
	const FVector TraceStart = SocketLocation + FVector(0.f, 0.f, TraceHalfDistance);
	const FVector TraceEnd = SocketLocation - FVector(0.f, 0.f, TraceHalfDistance);

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, Params);
	if (HitResult.bBlockingHit)
	{
		return (SocketLocation - HitResult.ImpactPoint).Z - FromBoneToBottom;
	}
	return 0.f;
}

void AP13TopDownCharacter::LegsIKFloorAlignment()
{
	// if (!CharacterAttribute->GetIsAlive())
	// {
	// 	IKLeftLegOffset = 0.f;
	// 	IKRightLegOffset = 0.f;
	// 	IKHitOffset = 0.f;
	// 	return;
	// }

	constexpr float MinDistanceThreshold = 12.f;
	constexpr float IKOffsetInterp = 10.f;

	float LeftOffset = GetIKSocketOffset("VB VB SK_Jedihunter_root_l_ankle");
	float RightOffset = GetIKSocketOffset("VB VB SK_Jedihunter_root_r_ankle");
	float HipOffset = 0.f;

	if (FMath::Abs(LeftOffset) > MinDistanceThreshold || FMath::Abs(RightOffset) > MinDistanceThreshold)
	{
		if (LeftOffset > RightOffset)
		{
			HipOffset = LeftOffset;
			LeftOffset = 0.f;
			RightOffset -= HipOffset;
		}
		else
		{
			HipOffset = RightOffset;
			RightOffset = 0.f;
			LeftOffset -= HipOffset;
		}
	}

	IKLeftLegOffset = FMath::FInterpTo(IKLeftLegOffset, LeftOffset, GetWorld()->GetTimeSeconds(), IKOffsetInterp);
	IKRightLegOffset = FMath::FInterpTo(IKRightLegOffset, RightOffset, GetWorld()->GetTimeSeconds(), IKOffsetInterp);
	IKHitOffset = FMath::FInterpTo(GetIKHipOffset(), HipOffset, GetWorld()->GetTimeSeconds(), IKOffsetInterp);
}

void AP13TopDownCharacter::ZoomSmoothly(const float DeltaTime, const float FinalLength)
{
	if (FMath::IsNearlyEqual(CameraBoom->TargetArmLength ,FinalLength, 0.1f))
	{
		GetWorld()->GetTimerManager().ClearTimer(ZoomTimer);
		return;
	}
	CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, FinalLength, DeltaTime, 1.f);
}
