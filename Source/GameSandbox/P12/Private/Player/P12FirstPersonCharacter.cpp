// Copyright (C) 2023, IKinder

#include "P12/Public/Player/P12FirstPersonCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "P12/Public/Component/Movement/P12BaseCharacterMovementComponent.h"
#include "P12/Public/Player/Controller/P12PlayerController.h"

AP12FirstPersonCharacter::AP12FirstPersonCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	/* AP12BaseCharacter */
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->SetCastHiddenShadow(true);
	GetBaseCharacterMovement()->SetRotationMode(false);
	bUseControllerRotationYaw = true;

	Camera->SetAutoActivate(false);
	CameraBoom->SetAutoActivate(false);
	CameraBoom->bUsePawnControlRotation = false;

	/* AP12FirstPersonCharacter */
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>("FirstPersonMeshComponent");
	FirstPersonMesh->SetupAttachment(RootComponent);
	FirstPersonMesh->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	FirstPersonMesh->CastShadow = false;
	FirstPersonMesh->bCastDynamicShadow = false;
	FirstPersonMesh->SetOnlyOwnerSee(true);

	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>("FirstPersonCameraComponent");
	FirstPersonCamera->SetupAttachment(FirstPersonMesh, SocketName);
	// FirstPersonCamera->AttachToComponent(FirstPersonMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
	FirstPersonCamera->bUsePawnControlRotation = true;
}

void AP12FirstPersonCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	CachedPlayerController = Cast<AP12PlayerController>(NewController);
}

void AP12FirstPersonCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AP12FirstPersonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsAnyMontagePlayingOn(FirstPersonMesh) && CachedPlayerController.IsValid())
	{
		FRotator TargetControlRotation = GetControlRotation();
		TargetControlRotation.Pitch = 0.f;
		constexpr float BlendSpeed = 300.f;
		TargetControlRotation = FMath::RInterpTo(GetControlRotation(), TargetControlRotation, DeltaTime, BlendSpeed);
		CachedPlayerController->SetControlRotation(TargetControlRotation);
	}
}

void AP12FirstPersonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

FRotator AP12FirstPersonCharacter::GetViewRotation() const
{
	FRotator Result = Super::GetViewRotation();

	if (IsAnyMontagePlayingOn(FirstPersonMesh))
	{
		const FRotator SocketRotation = FirstPersonMesh->GetSocketRotation(SocketName);
		/* Disable rotation from the controller while any montage is currently playing. */
		const float NewPitch = Result.Pitch + SocketRotation.Pitch;
		Result = FRotator(NewPitch, SocketRotation.Yaw, SocketRotation.Roll);
	}
	return Result;
}

void AP12FirstPersonCharacter::MoveInput(const FInputActionValue& Value)
{
	Super::MoveInput(Value);

	// GetBaseCharacterMovement()->SetRotationMode(false);
}

void AP12FirstPersonCharacter::LookInput(const FInputActionValue& Value)
{
	Super::LookInput(Value);
}

void AP12FirstPersonCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);

	const AP12FirstPersonCharacter* DefaultCharacter = GetDefault<AP12FirstPersonCharacter>(GetClass());
	FVector& MeshRelativeLocation = FirstPersonMesh->GetRelativeLocation_DirectMutable();
	MeshRelativeLocation.Z = DefaultCharacter->FirstPersonMesh->GetRelativeLocation().Z + HalfHeightAdjust;
}

void AP12FirstPersonCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);

	const AP12FirstPersonCharacter* DefaultCharacter = GetDefault<AP12FirstPersonCharacter>(GetClass());
	FVector& MeshRelativeLocation = FirstPersonMesh->GetRelativeLocation_DirectMutable();
	MeshRelativeLocation.Z = DefaultCharacter->FirstPersonMesh->GetRelativeLocation().Z;
}

void AP12FirstPersonCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	if (GetBaseCharacterMovement()->IsOnLadder())
	{
		TurnLadderLimits(true);
	}
	else if (PreviousCustomMode == static_cast<uint8>(EP12CustomMovementMode::CMOVE_Ladder))
	{
		TurnLadderLimits(false);
	}
}

void AP12FirstPersonCharacter::OnMantleHandle(const FP12MantleSettings& Settings, const float StartTime)
{
	Super::OnMantleHandle(Settings, StartTime);

	UAnimInstance* AnimInstance = FirstPersonMesh->GetAnimInstance();
	if (!AnimInstance || !Settings.Montage)
	{
		return;
	}
	SetIgnoreInput(true);
	const float Duration = AnimInstance->Montage_Play(Settings.Montage, 1.f, EMontagePlayReturnType::Duration, StartTime);

	FTimerDelegate MontageDelegate;
	MontageDelegate.BindUObject(this, &ThisClass::SetIgnoreInput, false);
	GetWorld()->GetTimerManager().SetTimer(MontageTimer, MontageDelegate, Duration, false);
}

bool AP12FirstPersonCharacter::IsAnyMontagePlayingOn(const USkeletalMeshComponent* MeshComponent) const
{
	const UAnimInstance* AnimInstance = MeshComponent->GetAnimInstance();
	return AnimInstance && AnimInstance->Montage_IsPlaying(nullptr);
}

void AP12FirstPersonCharacter::SetIgnoreInput(const bool bIgnore)
{
	if (!CachedPlayerController.IsValid())
	{
		return;
	}
	CachedPlayerController->SetIgnoreLookInput(bIgnore);
	CachedPlayerController->SetIgnoreMoveInput(bIgnore);
}

void AP12FirstPersonCharacter::SetCameraLimits(const FP12LadderCameraLimits Limits)
{
	APlayerCameraManager* CameraManager = CachedPlayerController->PlayerCameraManager;
	if (!CameraManager)
	{
		return;
	}
	CameraManager->ViewPitchMin = Limits.MinPitch;
	CameraManager->ViewPitchMax = Limits.MaxPitch;
	CameraManager->ViewYawMin = Limits.MinYaw;
	CameraManager->ViewYawMax = Limits.MaxYaw;
}

void AP12FirstPersonCharacter::ResetCameraLimits()
{
	APlayerCameraManager* DefaultCameraManager = CachedPlayerController->PlayerCameraManager->GetClass()->GetDefaultObject<APlayerCameraManager>();
	if (!DefaultCameraManager)
	{
		return;
	}
	const FP12LadderCameraLimits DefaultLimits = FP12LadderCameraLimits(DefaultCameraManager);
	SetCameraLimits(DefaultLimits);
}

void AP12FirstPersonCharacter::TurnLadderLimits(const bool bOn)
{
	if (!CachedPlayerController.IsValid())
	{
		return;
	}
	CachedPlayerController->SetIsIgnoreCameraPitch(bOn);
	bUseControllerRotationYaw = !bOn;
	bOn ? SetCameraLimits(LadderCameraLimits) : ResetCameraLimits();
}
