// Copyright © 2025, Ivan Kinder

#include "Player/P17CharacterHero.h"

#include "Project17.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Util/P17DebugHelper.h"

AP17CharacterHero::AP17CharacterHero()
{
	P17::Debug::Print("Working!");

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("SpringArmCameraBoomComponent");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 200.f;
	CameraBoom->SocketOffset = {0.f, 55.f, 65.f};
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>("FollowCameraComponent");
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	if (auto* CharMove = GetCharacterMovement())
	{
		CharMove->bOrientRotationToMovement = true;
		CharMove->RotationRate = {0.f, 500.f, 0.f};
		CharMove->MaxWalkSpeed = 400.f;
		CharMove->BrakingDecelerationWalking = 2000.f;
	}
}

void AP17CharacterHero::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	USkeletalMeshComponent* MainMesh = GetMesh();
	WARN_RETURN_IF(!MainMesh,)
	for (const TObjectPtr<USceneComponent>& Child : MainMesh->GetAttachChildren())
	{
		auto* SkinnedChild = Cast<USkinnedMeshComponent>(Child);
		CONTINUE_IF(!SkinnedChild)
		SkinnedChild->SetLeaderPoseComponent(MainMesh);
	}
}

void AP17CharacterHero::BeginPlay()
{
	Super::BeginPlay();
}
