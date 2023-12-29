// Copyright © 2024, IKinder

#include "P13/Public/Character/P13TopDownCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "P13/Public/Controller/P13PlayerController.h"
#include "P13/Public/Game/P13GameInstance.h"
#include "P13/Public/Weapon/P13Weapon.h"

AP13TopDownCharacter::AP13TopDownCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	// bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	CreateComponents();
}

void AP13TopDownCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	SetupCameraBoom();
}

void AP13TopDownCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (AP13PlayerController* PlayerController = Cast<AP13PlayerController>(NewController))
	{
		PlayerController->OnHitUnderCursorChanged.AddUObject(this, &ThisClass::OnHitUnderCursorChangedHandle);
	}
}

void AP13TopDownCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitWeapon(CurrentWeaponID);
}

void AP13TopDownCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AP13TopDownCharacter::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateMeshRotation();
}

void AP13TopDownCharacter::MoveInput(const FVector2D MoveVector)
{
	const FVector ForwardDirection = GetActorForwardVector();
	const FVector RightDirection = GetActorRightVector();

	AddMovementInput(ForwardDirection, MoveVector.Y);
	AddMovementInput(RightDirection, MoveVector.X);
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

void AP13TopDownCharacter::AimInput()
{
	/* Check if it can start aiming. */
	if (MovementState > EP13MovementState::Run)
	{
		return;
	}

	/* Cache the previous movement, to apply the correct one after aiming.
	 * I can`t hold my middle mouse button pressed, so we will switch aim mode on/off. */
	if (MovementState == EP13MovementState::Aim)
	{
		ChangeMovementState(PreviousMovementState);
		ZoomToCursor(false);
		return;
	}
	PreviousMovementState = MovementState;
	ChangeMovementState(EP13MovementState::Aim);
	ZoomToCursor(true);
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

void AP13TopDownCharacter::RotateTowardMovement(const FVector& Direction)
{
	const FRotator OldRotation = GetMesh()->GetComponentRotation();
	const FRotator NewRotation = Direction.Rotation() - FRotator(0.f, 90.f, 0.f);
	const FRotator InterpRotation = FMath::RInterpTo(OldRotation, NewRotation, GetWorld()->GetDeltaSeconds(), RotationRate);
	GetMesh()->SetWorldRotation(InterpRotation);
}

void AP13TopDownCharacter::FireInput(const bool bStart)
{
	/* Check  if there are any reasons why the character can't pull the trigger. */
	if (!CachedWeapon.IsValid() || !CheckCharacterCanFire())
	{
		return;
	}
	CachedWeapon->SetFireState(bStart);
}

void AP13TopDownCharacter::ReloadInput()
{
	if (!CachedWeapon.IsValid())
	{
		return;
	}
	CachedWeapon->TryReloadForce();
}

void AP13TopDownCharacter::UpdateCharacter()
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

	if (CachedWeapon.IsValid())
	{
		CachedWeapon->UpdateWeaponState(MovementState);
	}
}

void AP13TopDownCharacter::ChangeMovementState(const EP13MovementState NewMovementState)
{
	MovementState = NewMovementState;
	UpdateCharacter();
}

FVector AP13TopDownCharacter::GetLookAtCursorDirection() const
{
	return (HitUnderCursor.Location - CachedWeapon->GetShootLocation()).GetSafeNormal();
}

void AP13TopDownCharacter::OnHitUnderCursorChangedHandle(APlayerController* PlayerController, const FHitResult& HitResult)
{
	if (PlayerController != Controller)
	{
		return;
	}
	HitUnderCursor = HitResult;

	if (CachedWeapon.IsValid())
	{
		CachedWeapon->SetTargetLocation(HitResult.Location);
	}
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

void AP13TopDownCharacter::UpdateMeshRotation()
{
	if (!HitUnderCursor.bBlockingHit)
	{
		return;
	}
	const FVector CursorDirection = (HitUnderCursor.Location - GetActorLocation()).GetSafeNormal2D();
	RotateTowardMovement(CursorDirection);
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
	/*if (!CharacterAttribute->GetIsAlive())
	{
		IKLeftLegOffset = 0.f;
		IKRightLegOffset = 0.f;
		IKHitOffset = 0.f;
		return;
	}*/

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
	if (FMath::IsNearlyEqual(CameraBoom->TargetArmLength, FinalLength, 0.1f))
	{
		GetWorld()->GetTimerManager().ClearTimer(ZoomTimer);
		return;
	}
	CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, FinalLength, DeltaTime, 1.f);
}

void AP13TopDownCharacter::InitWeapon(const FName WeaponID)
{
	const UP13GameInstance* GameInstance = GetGameInstance<UP13GameInstance>();
	if (!GameInstance)
	{
		return;
	}
	FP13WeaponInfo* WeaponInfo = GameInstance->GetWeaponInfoByID(WeaponID);

	if (!WeaponInfo->Class)
	{
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Owner = GetOwner();
	SpawnParams.Instigator = GetInstigator();

	CachedWeapon = GetWorld()->SpawnActor<AP13Weapon>(WeaponInfo->Class, SpawnParams);
	if (!CachedWeapon.IsValid())
	{
		return;
	}
	CachedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocketName);
	CachedWeapon->WeaponInit(WeaponInfo, MovementState, GetMesh());
}

void AP13TopDownCharacter::ZoomToCursor(const bool bOn)
{
	if (!bOn)
	{
		GetWorld()->GetTimerManager().ClearTimer(AimTimer);
		CameraBoom->SetRelativeLocation(FVector::ZeroVector);
		return;
	}

	FTimerDelegate AimDelegate;
	AimDelegate.BindUObject(this, &ThisClass::ZoomToCursorSmoothly);
	GetWorld()->GetTimerManager().SetTimer(AimTimer, AimDelegate, GetWorld()->GetDeltaSeconds(), true);
}

void AP13TopDownCharacter::ZoomToCursorSmoothly() const
{
	const FVector UnsafeCursorDirection = HitUnderCursor.Location - GetActorLocation();
	const FVector DirectionToCursor = UnsafeCursorDirection.GetSafeNormal2D();
	const float DistanceToCursor = UnsafeCursorDirection.Size2D();

	constexpr float AimMin = 100.f;
	const float AimMax = CameraBoom->TargetArmLength / 2.f;
	constexpr float InterpSpeed = 2.f;

	FVector NewOffset = DirectionToCursor * (DistanceToCursor - AimMin);
	/* Set a safe zone around the character, to avoid offset flickering, when the cursor direction changes very quickly. */
	if (DistanceToCursor < AimMin)
	{
		NewOffset = FVector::ZeroVector;
	}
	/* Try not to lose sight of the character mesh. */
	else if (DistanceToCursor > AimMax)
	{
		NewOffset = DirectionToCursor * AimMax;
	}

	const FVector InterpOffset = FMath::VInterpTo(CameraBoom->GetRelativeLocation(), NewOffset, GetWorld()->GetDeltaSeconds(), InterpSpeed);
	CameraBoom->SetRelativeLocation(InterpOffset);
}

bool AP13TopDownCharacter::CheckCharacterCanFire()
{
	bool bResult = true;
	bResult = bResult && MovementState <= EP13MovementState::Run;

	return bResult;
}
