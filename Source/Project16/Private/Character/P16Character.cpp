// Copyright © 2025, Ivan Kinder

#include "Character/P16Character.h"

#include "Project16.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

AP16Character::AP16Character()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate              = FRotator{0.f, 400.f, 0.f};
	GetCharacterMovement()->bConstrainToPlane         = true;
	GetCharacterMovement()->bSnapToPlaneAtStart       = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw   = false;
	bUseControllerRotationRoll  = false;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->SetRelativeRotation(FRotator{-45.f, 0.f, 0.f});
	SpringArm->TargetArmLength         = 750.0f;
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->bEnableCameraLag        = true;
	SpringArm->bInheritPitch           = false;
	SpringArm->bInheritYaw             = false;
	SpringArm->bInheritRoll            = false;

	Camera = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	Camera->SetupAttachment(SpringArm.Get());
	Camera->bUsePawnControlRotation = false;
}

void AP16Character::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// All child meshes that are skinned should use the leader mesh animation blueprint.
	USkeletalMeshComponent* BodyMesh = GetMesh();
	EARLY_RETURN_IF(!BodyMesh)
	for (const TObjectPtr<USceneComponent>& Child : BodyMesh->GetAttachChildren())
	{
		USkinnedMeshComponent* SkinnedChild = Cast<USkinnedMeshComponent>(Child);
		CONTINUE_IF(!SkinnedChild)
		SkinnedChild->SetLeaderPoseComponent(BodyMesh);
	}
}

void AP16Character::BeginPlay()
{
	Super::BeginPlay();
}

void AP16Character::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}
