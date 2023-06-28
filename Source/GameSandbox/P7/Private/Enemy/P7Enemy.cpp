// Copyright (C) 2023, IKinder

#include "P7/Public/Enemy/P7Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"

AP7Enemy::AP7Enemy()
{
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

void AP7Enemy::BeginPlay()
{
	Super::BeginPlay();
}

void AP7Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP7Enemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AP7Enemy::GetHit(const FVector& ImpactPoint)
{
	DrawDebugPoint(GetWorld(), ImpactPoint, 20.f, FColor::Red, false, 5.f);
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("Hello")));
}

