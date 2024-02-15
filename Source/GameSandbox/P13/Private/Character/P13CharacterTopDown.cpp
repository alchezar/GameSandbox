// Copyright © 2024, IKinder

#include "P13/Public/Character/P13CharacterTopDown.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "P13/Public/Component/Actor/P13InventoryComponent.h"
#include "P13/Public/Controller/P13PlayerController.h"
#include "P13/Public/Game/P13PlayerState.h"
#include "P13/Public/Weapon/P13Weapon.h"

AP13CharacterTopDown::AP13CharacterTopDown()
{
	bReplicates = true;
	bAlwaysRelevant = true;
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

	TryLoadSavedColor(NewController);
	Client_ListenToControllerCursor(NewController);
	ShowInterface();
}

void AP13CharacterTopDown::BeginPlay()
{
	Super::BeginPlay();

	ListenToControllerCursor();
	ShowInterface();
	InventoryComponent->RefreshSlots();
}

void AP13CharacterTopDown::Tick(const float DeltaTime)
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

	AddMovementInput(FVector::ForwardVector, MoveVector.Y);
	AddMovementInput(FVector::RightVector, MoveVector.X);
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
	Server_ChangeMovementState(bStart ? EP13MovementState::Sprint : GetPreviousMovementState());
}

void AP13CharacterTopDown::AimInput()
{
	Server_ToggleAim();
}

void AP13CharacterTopDown::ZoomInput(const float Axis)
{
	if (!GetController()->IsLocalPlayerController())
	{
		return;
	}
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
	const FRotator OldRotation = GetActorRotation();
	const FRotator NewRotation = Direction.Rotation();
	const FRotator IntRotation = FMath::RInterpTo(OldRotation, NewRotation, GetWorld()->GetDeltaSeconds(), RotationRate);
	SetActorRotation(IntRotation);
}

void AP13CharacterTopDown::FireInput(const bool bStart)
{
	bTriggerPulled = bStart;
	Server_UpdateTargetLocation(HitUnderCursor.Location);
	Server_PullTrigger(bStart);
}

void AP13CharacterTopDown::ReloadInput()
{
	Server_TryReloadWeapon();
}

void AP13CharacterTopDown::SwitchWeaponInput(const bool bNext)
{
	Server_TakeNextWeapon(bNext);
}

void AP13CharacterTopDown::DropInput(const bool bTakeNext)
{
	Server_DropWeapon(bTakeNext);
}

FVector AP13CharacterTopDown::GetLookAtCursorDirection() const
{
	if (!CachedWeapon.IsValid())
	{
		return Super::GetLookAtCursorDirection();
	}
	return (HitUnderCursor.Location - GetActorLocation()).GetSafeNormal();
}

void AP13CharacterTopDown::OnWeaponReloadStartHandle(UAnimMontage* CharReloadAnim, const int32 WeaponIndex, const float ReloadingTime)
{
	Multicast_PlayAnimation(CharReloadAnim);
}

void AP13CharacterTopDown::OnDeathHandle(AController* Causer)
{
	if (Controller)
	{
		Controller->SetControlRotation(CameraBoom->GetComponentRotation());
	}

	/* Remove 1 life from player state and save player inventory */
	if (AP13PlayerState* DeadPlayerState = GetPlayerState<AP13PlayerState>())
	{
		DeadPlayerState->OnPlayerDied();
		DeadPlayerState->SavePlayerInventory(InventoryComponent->GetAllWeaponSlots(), InventoryComponent->GetAllAmmoSlots());
	}

	Super::OnDeathHandle(Causer);
}

void AP13CharacterTopDown::TryLoadSavedColor(AController* NewController)
{
	if (const AP13PlayerController* PlayerController = Cast<AP13PlayerController>(NewController))
	{
		TrueColor = PlayerController->GetPawnTrueColor();
	}

	Super::TryLoadSavedColor(NewController);
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
		CachedWeapon->SetTargetLocation(HitUnderCursor.Location);
	}
	if (bTriggerPulled)
	{
		Server_UpdateTargetLocation(HitUnderCursor.Location);
	}
}

void AP13CharacterTopDown::ShowInterface()
{
	if (AP13PlayerController* PlayerController = GetController<AP13PlayerController>())
	{
		PlayerController->ShowHeadsUpDisplay();
	}
}

void AP13CharacterTopDown::CreateTopDownComponents()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoomSpringArmComponent");
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritRoll = false;
	CameraBoom->bInheritYaw = true;

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
	if (!IsLocallyControlled())
	{
		return;
	}
	if (GetIsDead())
	{
		return;
	}
	if (!HitUnderCursor.bBlockingHit)
	{
		return;
	}
	const FVector ThisCursorDirection = (HitUnderCursor.Location - GetActorLocation()).GetSafeNormal2D();
	RotateTowardMovement(ThisCursorDirection);
	Server_RotateTowardMovement(ThisCursorDirection);
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
	if (!IsLocallyControlled())
	{
		return;
	}
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

void AP13CharacterTopDown::ToggleAim()
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
		Client_FocusOnCursor(false);
		return;
	}

	SavePreviousMovementState();
	ChangeMovementState(EP13MovementState::Aim);
	Client_FocusOnCursor(true);
}

void AP13CharacterTopDown::ListenToControllerCursor(AController* NewController)
{
	AP13PlayerController* PlayerController = Cast<AP13PlayerController>(NewController ? NewController : Controller);
	if (!PlayerController)
	{
		return;
	}
	if (!PlayerController->IsLocalPlayerController())
	{
		return;
	}
	if (PlayerController->OnHitUnderCursorChanged.IsBoundToObject(this))
	{
		return;
	}

	PlayerController->OnHitUnderCursorChanged.AddUObject(this, &ThisClass::OnHitUnderCursorChangedHandle);
}

void AP13CharacterTopDown::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AP13CharacterTopDown::Server_TakeNextWeapon_Implementation(const bool bNext) const
{
	TryTakeNextWeapon(bNext);
}

void AP13CharacterTopDown::Server_TryReloadWeapon_Implementation()
{
	TryReloadWeapon();
}

void AP13CharacterTopDown::Server_RotateTowardMovement_Implementation(const FVector& Direction)
{
	RotateTowardMovement(Direction);
	Multicast_RotateTowardMovement(Direction);
}

void AP13CharacterTopDown::Multicast_RotateTowardMovement_Implementation(const FVector& Direction)
{
	if (IsLocallyControlled())
	{
		return;
	}
	RotateTowardMovement(Direction);
}

void AP13CharacterTopDown::Client_ListenToControllerCursor_Implementation(AController* NewController)
{
	ListenToControllerCursor(NewController);
}

void AP13CharacterTopDown::Server_PullTrigger_Implementation(const bool bStart) const
{
	PullTrigger(bStart);
}

void AP13CharacterTopDown::Server_UpdateTargetLocation_Implementation(const FVector& TargetLocation)
{
	CachedWeapon->SetTargetLocation(TargetLocation);
}

void AP13CharacterTopDown::Server_ToggleAim_Implementation()
{
	ToggleAim();
}

void AP13CharacterTopDown::Client_FocusOnCursor_Implementation(const bool bOn)
{
	FocusOnCursor(bOn);
}
