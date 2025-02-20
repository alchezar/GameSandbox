// Copyright © 2025, Ivan Kinder

#include "Character/P16Character.h"

#include "Project16.h"

AP16Character::AP16Character()
{
	PrimaryActorTick.bCanEverTick = true;
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

void AP16Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
