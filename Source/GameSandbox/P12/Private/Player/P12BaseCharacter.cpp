// Copyright (C) 2023, IKinder

#include "P12/Public/Player/P12BaseCharacter.h"

#include "AIController.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Curves/CurveVector.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/SpringArmComponent.h"
#include "P12/Public/Actor/Equipment/Weapon/P12MeleeWeaponItem.h"
#include "P12/Public/Actor/Equipment/Weapon/P12RangeWeaponItem.h"
#include "P12/Public/Actor/Interactive/Environment/P12Ladder.h"
#include "P12/Public/Actor/Interactive/Interface/P12Interactable.h"
#include "P12/Public/Component/Actor/P12AttributeComponent.h"
#include "P12/Public/Component/Actor/P12EquipmentComponent.h"
#include "P12/Public/Component/Actor/P12InventoryComponent.h"
#include "P12/Public/Component/Actor/P12LedgeDetectionComponent.h"
#include "P12/Public/Component/MOvement/P12BaseCharacterMovementComponent.h"
#include "P12/Public/Game/P12HUD.h"
#include "P12/Public/Player/AnimNotify/P12AnimNotify_EnableRagdoll.h"
#include "P12/Public/Player/Controller/P12PlayerController.h"
#include "P12/Public/Subsystem/Streaming/P12StreamingSubsystemUtils.h"
#include "P12/Public/UI/P12AttributeProgressBarWidget.h"
#include "P12/Public/Util/P12CoreTypes.h"
#include "P12/Public/Util/P12Library.h"

AP12BaseCharacter::AP12BaseCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UP12BaseCharacterMovementComponent>(Super::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;

	BaseCharacterMovement = StaticCast<UP12BaseCharacterMovementComponent*>(GetCharacterMovement());
	BaseCharacterMovement->DefaultSetup();

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoomSpringArmComponent");
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SetRelativeLocation(FVector(0.f, 0.f, 50.f));
	CameraBoom->TargetArmLength = CameraArmLength.Walk;

	Camera = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	Camera->SetupAttachment(CameraBoom);

	LedgeDetection = CreateDefaultSubobject<UP12LedgeDetectionComponent>("LedgeDetectorComponent");
	CharacterAttribute = CreateDefaultSubobject<UP12AttributeComponent>("CharacterAttributeComponent");
	Equipment = CreateDefaultSubobject<UP12EquipmentComponent>("EquipmentComponent");
	Inventory = CreateDefaultSubobject<UP12InventoryComponent>("InventoryComponent");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthProgressBarWidgetComponent");
	HealthBar->SetupAttachment(RootComponent);
}

void AP12BaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	UP12StreamingSubsystemUtils::CheckCharacterOverlapStreamingSubsystemVolume(this);
	
	DefaultMeshLocation = GetMesh()->GetRelativeLocation();
	CharacterAttribute->OnDeath.AddUObject(this, &ThisClass::OnDeath);
	OnReloadComplete.AddUObject(this, &ThisClass::OnReloadCompleteHandle);
	InitAnimNotify();
	InitHealthProgress();
}

void AP12BaseCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AP12BaseCharacter::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	LegsIKFloorAlignment();
	TraceLineOfSight();
}

void AP12BaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AP12BaseCharacter::MoveInput(const FInputActionValue& Value)
{
	if (!BaseCharacterMovement->GetCanWalk())
	{
		BaseCharacterMovement->SetRotationMode(false);
		return;
	}
	
	const FVector2D MoveVector = Value.Get<FVector2D>();
	if (!Controller || FMath::IsNearlyZero(MoveVector.Size()))
	{
		BaseCharacterMovement->SetRotationMode(true);
		return;
	}
	BaseCharacterMovement->SetRotationMode(false);
	const FRotator ControllerYawRotator = FRotator(0.f, GetControlRotation().Yaw, 0.f);
	// const FVector ForwardDirection = FRotationMatrix(ControllerYawRotator).GetUnitAxis(EAxis::X);
	// const FVector RightDirection   = FRotationMatrix(ControllerYawRotator).GetUnitAxis(EAxis::Y);
	const FVector ForwardDirection = ControllerYawRotator.RotateVector(FVector::ForwardVector);
	const FVector RightDirection = ControllerYawRotator.RotateVector(FVector::RightVector);

	AddMovementInput(ForwardDirection, MoveVector.Y);
	AddMovementInput(RightDirection, MoveVector.X);
}

void AP12BaseCharacter::LookInput(const FInputActionValue& Value)
{
	const FVector2D LookVector = Value.Get<FVector2D>();
	if (!Controller || FMath::IsNearlyZero(LookVector.Size()))
	{
		return;
	}
	AddControllerYawInput(LookVector.X);
	AddControllerPitchInput(LookVector.Y);
}

void AP12BaseCharacter::JumpInput()
{
	if (!GetBaseCharacterMovement()->GetCanJump())
	{
		return;
	}
	
	Super::Jump();
}

void AP12BaseCharacter::MantleInput(const bool bForce)
{
	if (!GetBaseCharacterMovement()->GetCanMantle() && !bForce)
	{
		return;
	}
	
	FP12LedgeDescription LedgeDescription;
	if (!LedgeDetection->DetectLedge(OUT LedgeDescription) || BaseCharacterMovement->IsMantling())
	{
		return;
	}

	const float MantleHeight = (LedgeDescription.Location - GetActorLocation()).Z;
	const FP12MantleSettings MantleSettings = GetMantleSettings(MantleHeight);

	float MinRange;
	float MaxRange;
	MantleSettings.Curve->GetTimeRange(MinRange, MaxRange);
	const float Duration = MaxRange - MinRange;

	const FVector2D Source = {MantleSettings.MinHeight, MantleSettings.MaxHeight};
	const FVector2D Target = {MantleSettings.MinHeightStartTime, MantleSettings.MaxHeightStartTime};
	const float StartTime = FMath::GetMappedRangeValueUnclamped(Source, Target, MantleHeight);

	const FVector InitAnimLocation = LedgeDescription.Location - MantleSettings.AnimationCorrectionZ * FVector::UpVector + MantleSettings.AnimationCorrectionXY * LedgeDescription.Normal;

	const FP12MantleMovementParams MantleParams = {GetActorLocation(), GetActorRotation(), LedgeDescription.Location, LedgeDescription.Rotation, Duration, StartTime, MantleSettings.Curve, InitAnimLocation};

	GetBaseCharacterMovement()->SetMantle(true);

	if (IsLocallyControlled() || HasAuthority())
	{
		GetBaseCharacterMovement()->StartMantle(MantleParams);

	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance)
	{
		return;
	}
	AnimInstance->Montage_Play(MantleSettings.Montage, 1.f, EMontagePlayReturnType::Duration, StartTime);
	// AnimInstance->Montage_JumpToSection("Mantle_High", MantleSettings.Montage);
	OnMantleHandle(MantleSettings, StartTime);
}

void AP12BaseCharacter::CrouchInput()
{
	bCrouch = !bCrouch;
	bCrouch ? Crouch() : UnCrouch();
	/*              ↓            ↓ 
	 * ::OnStartCrouch(...) || ::OnEndCrouch(...) */

	ChangeCameraArmLength(bCrouch, CameraArmLength.Crouch);
}

void AP12BaseCharacter::RunInput(const bool bRunRequest)
{
	if (bCrouch)
	{
		bCrouch = false;
		UnCrouch();
	}
	if (bRunRequest && !bRunning && !GetCanRun())
	{
		return;
	}
	bRunning = bRunRequest;
	BaseCharacterMovement->ToggleMaxSpeed(bRunRequest);

	ChangeCameraArmLength(bRunRequest, CameraArmLength.Run);
}

void AP12BaseCharacter::LadderJumpInput()
{
	if (GetBaseCharacterMovement()->IsOnLadder())
	{
		GetBaseCharacterMovement()->DetachFromLadder(EP12DetachFromLadderMethod::JumpOff);
		return;
	}
	const AP12Ladder* Ladder = GetAvailableLadder();
	if (!Ladder)
	{
		return;
	}
	if (Ladder->GetIsOnTop() && AttachFromTopMontage)
	{
		PlayAnimMontage(AttachFromTopMontage);
	}
	GetBaseCharacterMovement()->AttachToLadder(Ladder);
}

void AP12BaseCharacter::LadderClimbInput(const FInputActionValue& Value)
{
	const float AxisValue = Value.Get<float>();
	if (!GetBaseCharacterMovement()->IsOnLadder() || FMath::IsNearlyZero(AxisValue))
	{
		return;
	}
	const FVector LadderUpVector = GetBaseCharacterMovement()->GetCurrentLadder()->GetActorUpVector();
	AddMovementInput(LadderUpVector, AxisValue);
}

void AP12BaseCharacter::ChangeCameraArmLength(const bool bStart, const float NewArmLength, const float NewFOV)
{
	const float TargetLength = bStart ? NewArmLength : GetDefaultCameraArmLength();
	const float TargetFOV = bStart ? NewFOV : GetDefaultCameraFOV();
	FTimerDelegate RunDelegate;
	RunDelegate.BindUObject(this, &ThisClass::SmoothlyChangeCameraArmLength, bStart, TargetLength, TargetFOV);
	GetWorld()->GetTimerManager().SetTimer(RunTimer, RunDelegate, GetWorld()->GetDeltaSeconds(), true);
}

void AP12BaseCharacter::SmoothlyChangeCameraArmLength(const bool bRunStart, const float TargetLength, const float TargetFOV)
{
	constexpr float InterpSpeed = 2.f;
	CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, TargetLength, GetWorld()->GetDeltaSeconds(), InterpSpeed);
	Camera->FieldOfView = FMath::FInterpTo(Camera->FieldOfView, TargetFOV, GetWorld()->GetDeltaSeconds(), InterpSpeed);
	if (FMath::IsNearlyEqual(CameraBoom->TargetArmLength, TargetLength))
	{
		GetWorld()->GetTimerManager().ClearTimer(RunTimer);
	}
}

void AP12BaseCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);

	CameraBoom->TargetOffset = FVector(0.f, 0.f, HalfHeightAdjust);
}

void AP12BaseCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);

	CameraBoom->TargetOffset = FVector(0.f, 0.f, 0.f);
}

bool AP12BaseCharacter::CanJumpInternal_Implementation() const
{
	return bCrouch || !GetBaseCharacterMovement()->IsMantling() || Super::CanJumpInternal_Implementation();
}

void AP12BaseCharacter::OnJumped_Implementation()
{
	if (bCrouch)
	{
		bCrouch = false;
		UnCrouch();
	}
}

void AP12BaseCharacter::OnMantleHandle(const FP12MantleSettings& Settings, const float StartTime)
{
	
}

bool AP12BaseCharacter::GetCanRun() const
{
	if (bAiming)
	{
		return false;
	}
	return true;
}

UP12BaseCharacterMovementComponent* AP12BaseCharacter::GetBaseCharacterMovement() const
{
	return BaseCharacterMovement.Get();
}

FRotator AP12BaseCharacter::GetLocalAimOffset()
{
	const FVector AimDirectionWorld = GetBaseAimRotation().Vector();
	const FVector AimDirectionLocal = GetTransform().InverseTransformVectorNoScale(AimDirectionWorld);
	const FRotator AimRotation = AimDirectionLocal.ToOrientationRotator();

	return AimRotation;
}

float AP12BaseCharacter::GetIKSocketOffset(const FName& VirtualBoneName, const float TraceHalfDistance, const float FromBoneToBottom)
{
	const FVector SocketLocation = GetMesh()->GetBoneLocation(VirtualBoneName);
	const FVector TraceStart = SocketLocation + FVector(0.f, 0.f, TraceHalfDistance);
	const FVector TraceEnd = SocketLocation - FVector(0.f, 0.f, TraceHalfDistance);

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, Params);
	UP12Library::DrawDebugLineTrace(GetWorld(), HitResult, UP12Library::GetCanDrawDebugLegAlignment());
	if (HitResult.bBlockingHit)
	{
		return (SocketLocation - HitResult.ImpactPoint).Z - FromBoneToBottom;
	}
	return 0.f;
}

void AP12BaseCharacter::LegsIKFloorAlignment()
{
	if (!CharacterAttribute->GetIsAlive())
	{
		IKLeftLegOffset = 0.f; 
		IKRightLegOffset = 0.f; 
		IKHitOffset = 0.f;
		return;
	}
	
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

const FP12MantleSettings& AP12BaseCharacter::GetMantleSettings(const float LedgeHeight) const
{
	return LedgeHeight > LowMantleMaxHeight ? HighMantleSettings : LowMantleSettings;
}

void AP12BaseCharacter::RegisterInteractiveActor(AP12InteractiveActor* NewInteractiveActor)
{
	if (AvailableInteractiveActors.Contains(NewInteractiveActor))
	{
		return;
	}
	AvailableInteractiveActors.AddUnique(NewInteractiveActor);
}

void AP12BaseCharacter::UnregisterInteractiveActor(AP12InteractiveActor* OldInteractiveActor)
{
	if (!AvailableInteractiveActors.Contains(OldInteractiveActor))
	{
		return;
	}
	AvailableInteractiveActors.RemoveSingleSwap(OldInteractiveActor);
}

const AP12Ladder* AP12BaseCharacter::GetAvailableLadder() const
{
	const AP12Ladder* Result = nullptr;
	for (const AP12InteractiveActor* InteractiveActor : AvailableInteractiveActors)
	{
		if (!InteractiveActor->IsA<AP12Ladder>())
		{
			continue;
		}
		Result = StaticCast<const AP12Ladder*>(InteractiveActor);
		break;
	}
	return Result;
}

void AP12BaseCharacter::InitAnimNotify()
{
	check(DeathMontage)
	TArray<FAnimNotifyEvent> AnimNotifies = DeathMontage->Notifies;
	for (const auto AnimNotify : AnimNotifies)
	{
		if (UP12AnimNotify_EnableRagdoll* EnableRagdollNotify = Cast<UP12AnimNotify_EnableRagdoll>(AnimNotify.Notify))
		{
			StopAnimMontage(DeathMontage);
			EnableRagdollNotify->OnEnableRagdoll.AddUObject(this, &ThisClass::OnEnableRagdollHandle);
		}
	}
}

void AP12BaseCharacter::OnEnableRagdollHandle(USkeletalMeshComponent* SkeletalMeshComponent)
{
	if (SkeletalMeshComponent != GetMesh())
	{
		return;
	}
	EnableRagdoll();
}

void AP12BaseCharacter::OnDeath()
{
	StopAnimMontage();
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->DisableMovement();
		GetCharacterMovement()->MovementState.bCanJump = false;
	}
	
	if (DeathMontage)
	{
		PlayAnimMontage(DeathMontage);
	}
	else
	{
		EnableRagdoll();
	}
}

void AP12BaseCharacter::EnableRagdoll()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName("Ragdoll");
	DetachFromControllerPendingDestroy();
	SetLifeSpan(10.f);
}

void AP12BaseCharacter::Falling()
{
	Super::Falling();
	GetBaseCharacterMovement()->bNotifyApex = true;
}

void AP12BaseCharacter::NotifyJumpApex()
{
	Super::NotifyJumpApex();
	CurrentFallApex = GetActorLocation();
}

void AP12BaseCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	const float FallHeight = (CurrentFallApex - GetActorLocation()).Z;
	if (FallDamageCurve)
	{
		const float DamageAmount = FallDamageCurve->GetFloatValue(FallHeight);
		TakeDamage(DamageAmount, FDamageEvent{}, GetController(), Hit.GetActor());
	}
}

void AP12BaseCharacter::FireInput(const bool bStart)
{
	if (Equipment->GetIsSelectingWeapon())
	{
		Equipment->ConfirmWeaponSelection();
		return;
	}
	
	bFiring = bStart;
	AP12RangeWeaponItem* CurrentWeapon = Equipment->GetCurrentEquippedWeapon();
	if (!CurrentWeapon)
	{
		bFiring = false;
		return;
	}
	CurrentWeapon->FireInput(bStart);
}

void AP12BaseCharacter::AimInput(const bool bStart)
{
	bAiming = bStart;
	OnAimingStateChanged.Broadcast(bStart);
	AP12RangeWeaponItem* CurrentWeapon = Equipment->GetCurrentEquippedWeapon();
	if (!CurrentWeapon)
	{
		bAiming = false;
		return;
	}
	CurrentWeapon->AimInput(bAiming);
	ChangeCameraArmLength(bAiming, CameraArmLength.Aim, CurrentWeapon->GetAimingFOV());
}

void AP12BaseCharacter::ReloadInput()
{
	AP12RangeWeaponItem* CurrentWeapon = Equipment->GetCurrentEquippedWeapon();
	if (!CurrentWeapon)
	{
		return;
	}
	bReloading = true;
	CurrentWeapon->StartReloading();
}

void AP12BaseCharacter::OnReloadCompleteHandle(bool bReloaded)
{
	bReloading = false;
}

void AP12BaseCharacter::EquipItemInput(const bool bNext)
{
	bNext ? Equipment->EquipNextItem() : Equipment->EquipPreviousItem();
}

float AP12BaseCharacter::GetDefaultCameraArmLength() const
{
	if (bAiming)
	{
		if (bCrouch)
		{
			return CameraArmLength.Crouch;
		}
		if(bRunning)
		{
			return CameraArmLength.Run;
		}
	}
	return CameraArmLength.Walk;
}

float AP12BaseCharacter::GetDefaultCameraFOV() const
{
	const UCameraComponent* DefaultCamera = Camera->GetClass()->GetDefaultObject<UCameraComponent>();
	if (!DefaultCamera)
	{
		return 90.f ;
	}
	return DefaultCamera->FieldOfView;
}

float AP12BaseCharacter::GetHeathPercent()
{
	return CharacterAttribute->GetHealthPercent();
}

float AP12BaseCharacter::PlayEquippingItem(UAnimMontage* EquipMontage)
{
	bEquipping = true;
	return PlayAnimMontage(EquipMontage);
}

void AP12BaseCharacter::EndEquippingItem()
{
	bEquipping = false;
}

void AP12BaseCharacter::EquipThrowableInput()
{
	Equipment->EquipItemInSlot(EP12EquipmentSlot::ThrowableSlot);
}

void AP12BaseCharacter::PrimaryMeleeInput()
{
	AP12MeleeWeaponItem* MeleeWeapon = Equipment->GetCurrentMeleeWeapon();
	if (!MeleeWeapon)
	{
		return;
	}
	MeleeWeapon->StartAttack(EP12MeleeAttackType::Primary);
}

void AP12BaseCharacter::SecondaryMeleeInput()
{
	AP12MeleeWeaponItem* MeleeWeapon = Equipment->GetCurrentMeleeWeapon();
	if (!MeleeWeapon)
	{
		return;
	}
	MeleeWeapon->StartAttack(EP12MeleeAttackType::Secondary);
}

void AP12BaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (AAIController* AIController = Cast<AAIController>(NewController))
	{
		const FGenericTeamId TeamID = {static_cast<uint8>(Team)};
		AIController->SetGenericTeamId(TeamID);
	}

	Client_ShowInterface();
}

AP12HUD* AP12BaseCharacter::GetHUD() const
{
	AController* CurrentController = GetController();
	if (!CurrentController || CurrentController->GetLocalRole() < ROLE_AutonomousProxy)
	{
		return nullptr;
	}
	const APlayerController* PlayerController = Cast<APlayerController>(CurrentController);
	if (!PlayerController)
	{
		return nullptr;
	}
	return PlayerController->GetHUD<AP12HUD>();
}

FGenericTeamId AP12BaseCharacter::GetGenericTeamId() const
{
	// return IGenericTeamAgentInterface::GetGenericTeamId()
	return FGenericTeamId(static_cast<uint8>(Team));
}

void AP12BaseCharacter::Client_ShowInterface_Implementation()
{
	AP12HUD* HUD = GetHUD();
	if (!HUD)
	{
		return;
	}
	HUD->ShowGameScreenFor(this);
}

void AP12BaseCharacter::TraceLineOfSight()
{
	if (!IsPlayerControlled())
	{
		return;
	}
	const AP12PlayerController* PlayerController = GetController<AP12PlayerController>();
	if (!PlayerController)
	{
		return;
	}
	FVector ViewLocation;
	FRotator ViewRotation;
	PlayerController->GetPlayerViewPoint(ViewLocation, ViewRotation);
	const FVector ViewDirection = ViewRotation.Vector();
	const FVector TraceEnd = ViewLocation + ViewDirection * LineOfSightDistance;

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, ViewLocation, TraceEnd, ECC_Visibility);
	if (!HitResult.bBlockingHit)
	{
		if (bInteractableFound)
		{
			bInteractableFound = false;
			OnInteractableObjectFound.Broadcast(false);
		}
		if (LineOfSightObject.GetObject())
		{
			LineOfSightObject = nullptr;
		}
		return;
	}
	
	LineOfSightObject = HitResult.GetActor();
	// FName ActionName = NAME_None;
	if (LineOfSightObject.GetInterface())
	{
		// ActionName = LineOfSightObject->GetActionEventName();
		if (!bInteractableFound)
		{
			bInteractableFound = true;
			OnInteractableObjectFound.Broadcast(true);
		}
	}
}

void AP12BaseCharacter::Interact()
{
	if (LineOfSightObject.GetInterface())
	{
		LineOfSightObject->Interact(this);
	}
}

void AP12BaseCharacter::InitHealthProgress()
{
	UP12AttributeProgressBarWidget* Widget = Cast<UP12AttributeProgressBarWidget>(HealthBar->GetWidget());
	if (!Widget || (IsPlayerControlled() && IsLocallyControlled()))
	{
		HealthBar->SetVisibility(false);
		return;
	}
	OnHealthChange.AddUObject(Widget, &UP12AttributeProgressBarWidget::OnHealthChangedHandle);
	CharacterAttribute->OnDeath.AddLambda([=]()
	{
		HealthBar->SetVisibility(false);
	});
	Widget->CacheOwner(this);
	Widget->SetProgressPercentage(CharacterAttribute->GetHealthPercent());
}

// void AP12BaseCharacter::AddEquipmentItemToSlot(const TSubclassOf<AP12EquipableItem>& EquipableItemClass, int32 SlotIndex)
// {
// 	Equipment->AddEquipmentItemToSlot(EquipableItemClass, SlotIndex);
// }

bool AP12BaseCharacter::PickupItem(const TWeakObjectPtr<UP12InventoryItem> Item)
{
	bool Result = false;
	if (Inventory->HasFreeSlot())
	{
		Result = Inventory->TryAddItem(Item, 1);
	}
	
	return Result;
}

void AP12BaseCharacter::UseInventory(APlayerController* PlayerController)
{
	if (!IsPlayerControlled())
	{
		return;
	}
	
	if (!Inventory->GetIsViewVisible())
	{
		Inventory->OpenViewInventory(PlayerController);
		Equipment->OpenViewEquipment(PlayerController);
		
		FInputModeGameAndUI GameAndUIMode;
		GameAndUIMode.SetHideCursorDuringCapture(false);
		PlayerController->SetInputMode(GameAndUIMode);
		PlayerController->SetShowMouseCursor(true);
	}
	else
	{
		Inventory->CloseViewInventory();
		Equipment->CloseViewEquipment();
		
		PlayerController->SetInputMode(FInputModeGameOnly());	
		PlayerController->SetShowMouseCursor(false);
	}
}

void AP12BaseCharacter::ConfirmWeaponSelection(AP12PlayerController* AP12PlayerController)
{
	if (Equipment->GetIsSelectingWeapon())
	{
		Equipment->ConfirmWeaponSelection();
	}
}

void AP12BaseCharacter::OnLevelDeserialized_Implementation()
{
	
}
