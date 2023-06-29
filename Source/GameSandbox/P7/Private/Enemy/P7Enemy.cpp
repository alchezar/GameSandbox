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
	const FVector Forward = GetActorForwardVector();
	FVector ToHit = (ImpactPoint - GetActorLocation()).GetSafeNormal();
	ToHit.Z = Forward.Z;
	const float Sign  = FMath::Sign(FVector::CrossProduct(Forward, ToHit).Z);
	const float Angle = Sign * FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(Forward, ToHit)));

	FName SectionName = FName("FromBack");
	if		(Angle >=  -45.f && Angle <   45.f) SectionName = FName("FromFront");
	else if (Angle >= -135.f && Angle <  -45.f) SectionName = FName("FromLeft");
	else if (Angle >=   45.f && Angle <  135.f) SectionName = FName("FromRight");
	PlayHitReactMontage(SectionName);
}

void AP7Enemy::PlayHitReactMontage(const FName& SectionName)
{
	PlayAnimMontage(HitReactMontage, 1.f, SectionName);
}
