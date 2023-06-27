// Copyright (C) 2023, IKinder

#include "P7/Public/Item/Weapon/P7Weapon.h"
#include "Components/SphereComponent.h"

AP7Weapon::AP7Weapon()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AP7Weapon::BeginPlay()
{
	Super::BeginPlay();
}

void AP7Weapon::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP7Weapon::Equip(USceneComponent* InParent, const FName SocketName)
{
	SetItemState(EItemState::EIS_Equipped);
	SphereTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);

	const FAttachmentTransformRules AllRules(EAttachmentRule::SnapToTarget, true);		
	ItemMesh->AttachToComponent(InParent, AllRules, SocketName);
	
	ItemMesh->AddRelativeLocation(FVector(0.f, 2.f, 0.f));
	ItemMesh->AddRelativeRotation(FRotator(25.f, 0.f, 10.f));
}