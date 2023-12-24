// Copyright © 2024, IKinder

#include "P13/Public/Character/P13TopDownCharacter.h"

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
