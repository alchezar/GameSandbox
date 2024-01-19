// Copyright © 2024, IKinder

#include "P13/Public/Character/P13CharacterTopDown.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "P13/Public/Component/Actor/P13InventoryComponent.h"
#include "P13/Public/Controller/P13PlayerController.h"
#include "P13/Public/Weapon/P13Weapon.h"

AP13CharacterTopDown::AP13CharacterTopDown()
{
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	
	CreateTopDownComponents();
}

void AP13CharacterTopDown::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SetupCameraBoom();
}

void AP13CharacterTopDown::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	if (AP13PlayerController* PlayerController = Cast<AP13PlayerController>(NewController))
	{
		PlayerController->OnHitUnderCursorChanged.AddUObject(this, &ThisClass::OnHitUnderCursorChangedHandle);
	}
}

void AP13CharacterTopDown::BeginPlay()
{
	Super::BeginPlay();
}

void AP13CharacterTopDown::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateMeshRotation();
}

void AP13CharacterTopDown::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AP13CharacterTopDown::MoveInput(const FVector2D MoveVector)
{
	if (GetIsDead())
	{
		return;
	}

	const FVector ForwardDirection = GetActorForwardVector();
	const FVector RightDirection = GetActorRightVector();

	AddMovementInput(ForwardDirection, MoveVector.Y);
	AddMovementInput(RightDirection, MoveVector.X);
}

void AP13CharacterTopDown::SprintInput(const bool bStart)
{
	/* Check if it can start sprinting. */
	if (MovementState < EP13MovementState::Walk)
	{
		return;
	}

	/* Cache the previous movement, to apply the correct one after the sprint is complete. */
	if (bStart)
	{
		SavePreviousMovementState();
	}
	ChangeMovementState(bStart ? EP13MovementState::Sprint : GetPreviousMovementState());
}

void AP13CharacterTopDown::AimInput()
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
		ChangeMovementState(GetPreviousMovementState());
		FocusOnCursor(false);
		return;
	}
	SavePreviousMovementState();
	ChangeMovementState(EP13MovementState::Aim);
	FocusOnCursor(true);
}

void AP13CharacterTopDown::ZoomInput(const float Axis)
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

void AP13CharacterTopDown::RotateTowardMovement(const FVector& Direction)
{
	const FRotator OldRotation = GetMesh()->GetComponentRotation();
	const FRotator NewRotation = Direction.Rotation() - FRotator(0.f, 90.f, 0.f);
	const FRotator InterpRotation = FMath::RInterpTo(OldRotation, NewRotation, GetWorld()->GetDeltaSeconds(), RotationRate);
	GetMesh()->SetWorldRotation(InterpRotation);
}

void AP13CharacterTopDown::FireInput(const bool bStart)
{
	/* Check  if there are any reasons why the character can't pull the trigger. */
	if (!CachedWeapon.IsValid() || !CheckCharacterCanFire())
	{
		return;
	}
	CachedWeapon->SetFireState(bStart);
}

void AP13CharacterTopDown::ReloadInput()
{
	if (!CachedWeapon.IsValid())
	{
		return;
	}
	CachedWeapon->TryReload();
}

void AP13CharacterTopDown::SwitchWeaponInput(const bool bNext)
{
	check(InventoryComponent)
	if (InventoryComponent->GetWeaponSlotsCount() < 2)
	{
		return;
	}
	if (!CachedWeapon.IsValid())
	{
		return;
	}
	CachedWeapon->AbortReloading();

	const FP13WeaponDynamicInfo OndInfo = CachedWeapon->GetDynamicInfo();
	const int32 OldIndex = InventoryComponent->GetCurrentWeaponIndex();
	const int32 NextDirection = bNext ? 1 : -1;
	const int32 NewIndex = OldIndex + NextDirection;

	InventoryComponent->TrySwitchWeaponToIndex(NewIndex, OldIndex, OndInfo);
}

void AP13CharacterTopDown::DropInput(const bool bTakeNext)
{
	DropWeapon(bTakeNext);
}

FVector AP13CharacterTopDown::GetLookAtCursorDirection() const
{
	if (!CachedWeapon.IsValid())
	{
		return Super::GetLookAtCursorDirection();
	}
	return (HitUnderCursor.Location - GetActorLocation()).GetSafeNormal();
}

void AP13CharacterTopDown::OnHitUnderCursorChangedHandle(APlayerController* PlayerController, const FHitResult& HitResult)
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

void AP13CharacterTopDown::OnDeathHandle(AController* Causer)
{
	if (Controller)
	{
		Controller->SetControlRotation(CameraBoom->GetComponentRotation());
	}
	
	Super::OnDeathHandle(Causer);
}

void AP13CharacterTopDown::CreateTopDownComponents()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoomSpringArmComponent");
	CameraBoom->SetupAttachment(RootComponent);

	TopDownCamera = CreateDefaultSubobject<UCameraComponent>("TopDownCameraComponent");
	TopDownCamera->SetupAttachment(CameraBoom);
}

void AP13CharacterTopDown::SetupCameraBoom() const
{
	CameraBoom->TargetArmLength = Height;
	CameraBoom->bEnableCameraLag = bFlow;
	CameraBoom->CameraLagSpeed = SmoothCoefficient;
	CameraBoom->SetRelativeRotation(CameraRotation);
}

void AP13CharacterTopDown::ShakeCamera() const
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController)
	{
		return;
	}
	PlayerController->ClientStartCameraShake(ShakeClass);
}

void AP13CharacterTopDown::UpdateMeshRotation()
{
	if (GetIsDead())
	{
		return;
	}
	if (!HitUnderCursor.bBlockingHit)
	{
		return;
	}
	const FVector CursorDirection = (HitUnderCursor.Location - GetActorLocation()).GetSafeNormal2D();
	RotateTowardMovement(CursorDirection);
}

void AP13CharacterTopDown::ZoomSmoothly(const float DeltaTime, const float FinalLength)
{
	if (FMath::IsNearlyEqual(CameraBoom->TargetArmLength, FinalLength, 0.1f))
	{
		GetWorld()->GetTimerManager().ClearTimer(ZoomTimer);
		return;
	}
	CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, FinalLength, DeltaTime, 1.f);
}

void AP13CharacterTopDown::FocusOnCursor(const bool bOn)
{
	if (!bOn)
	{
		GetWorld()->GetTimerManager().ClearTimer(AimTimer);
		CameraBoom->SetRelativeLocation(FVector::ZeroVector);
		return;
	}

	FTimerDelegate AimDelegate;
	AimDelegate.BindUObject(this, &ThisClass::FocusOnCursorSmoothly);
	GetWorld()->GetTimerManager().SetTimer(AimTimer, AimDelegate, GetWorld()->GetDeltaSeconds(), true);
}

void AP13CharacterTopDown::FocusOnCursorSmoothly() const
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
