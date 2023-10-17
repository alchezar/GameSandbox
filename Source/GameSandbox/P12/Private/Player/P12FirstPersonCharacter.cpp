// Copyright (C) 2023, IKinder

#include "P12/Public/Player/P12FirstPersonCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "P12/Public/Component/Movement/P12BaseCharacterMovementComponent.h"

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
	FirstPersonCamera->AttachToComponent(FirstPersonMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
	FirstPersonCamera->bUsePawnControlRotation = true;
}

void AP12FirstPersonCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AP12FirstPersonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP12FirstPersonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AP12FirstPersonCharacter::MoveInput(const FInputActionValue& Value)
{
	Super::MoveInput(Value);

	GetBaseCharacterMovement()->SetRotationMode(false);
}

