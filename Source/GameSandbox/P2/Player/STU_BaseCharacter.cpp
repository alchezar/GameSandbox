// Copyright (C) 2023, IKinder

#include "STU_BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameSandbox/P2/Component/STU_CharacterMovementComponent.h"
#include "GameSandbox/P2/Component/STU_HealthComponent.h"
#include "GameSandbox/P2/Component/STU_WeaponComponent.h"

ASTU_BaseCharacter::ASTU_BaseCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<USTU_CharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw                         = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	HealthComponent     = CreateDefaultSubobject<USTU_HealthComponent>("HealthComponent");

	WeaponComponent = CreateDefaultSubobject<USTU_WeaponComponent>("WeaponComponent");
}

void ASTU_BaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	check(HealthComponent);
	check(GetCharacterMovement());

	OnHealthChangedHandle(HealthComponent->GetHealth(), 1.f);
	HealthComponent->OnDeath.AddUObject(this, &ASTU_BaseCharacter::OnDeathHandle);
	HealthComponent->OnHealthChanged.AddUObject(this, &ASTU_BaseCharacter::OnHealthChangedHandle);
	LandedDelegate.AddDynamic(this, &ASTU_BaseCharacter::LandedHandle);
}

void ASTU_BaseCharacter::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ASTU_BaseCharacter::GetIsRunning() const
{
	return false;
}

void ASTU_BaseCharacter::LandedHandle(const FHitResult& Hit)
{
	const double FallVelocity = -GetVelocity().Z;
	if (FallVelocity < LandedDamageVelocity.X) return;

	const double FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocity);
	TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);
}

void ASTU_BaseCharacter::OnHealthChangedHandle(const float Health, float HealthDelta)
{
	
}

void ASTU_BaseCharacter::OnDeathHandle()
{
	const int32 RandomElement = FMath::RandRange(0, DeathAnimations.Num() - 1);
	PlayAnimMontage(DeathAnimations[RandomElement]);

	GetCharacterMovement()->DisableMovement();
	SetLifeSpan(LifeSpanOnDeath);

	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	WeaponComponent->StopFire();

	/* After every death check if there is even one player left in the team   */
	// Cast<ASTU_GameModeBase>(GetWorld()->GetAuthGameMode())->StopRoundWhenAllTeamDead();
}

void ASTU_BaseCharacter::SetPlayerColor(const FLinearColor& NewTeamColor)
{
	USkeletalMeshComponent* CharacterMesh = GetMesh();
	if (!CharacterMesh) return;

	UMaterialInstanceDynamic* MaterialDynamic1 = CharacterMesh->CreateAndSetMaterialInstanceDynamic(0);
	UMaterialInstanceDynamic* MaterialDynamic2 = CharacterMesh->CreateAndSetMaterialInstanceDynamic(1);
	UMaterialInstanceDynamic* MaterialDynamic3 = CharacterMesh->CreateAndSetMaterialInstanceDynamic(2);

	MaterialDynamic1->SetVectorParameterValue(MaterialColorName, NewTeamColor);
	MaterialDynamic2->SetVectorParameterValue(MaterialColorName, NewTeamColor);
	MaterialDynamic3->SetVectorParameterValue(MaterialColorName, NewTeamColor);
}

float ASTU_BaseCharacter::GetMovementDirection() const
{
	if (GetVelocity().IsZero()) return 0.f;
	const auto VelocityNormal = GetVelocity().GetSafeNormal();
	const auto AngleBetween   = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
	const auto CrossProduct   = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
	return FMath::RadiansToDegrees(AngleBetween) * FMath::Sign(CrossProduct.Z);
}