// Copyright (C) 2023, IKinder

#include "Player/TG_BaseCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon/TG_Gun.h"

ATG_BaseCharacter::ATG_BaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATG_BaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	OnTakeAnyDamage.AddDynamic(this, &ThisClass::OnTakeAnyDamageHandle);
	
	if (WeaponClass)
	{
		CurrentWeapon = GetWorld()->SpawnActor<ATG_Gun>(WeaponClass);
		if (!CurrentWeapon) return;
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), HandSocketName);
		CurrentWeapon->SetActorRelativeLocation(FVector(-2.f, -12.f, -1.f));
		CurrentWeapon->SetActorRelativeRotation(FRotator(0.f, 0.f, 0.f));
		CurrentWeapon->SetAnimInstance(GetMesh()->GetAnimInstance());
		CurrentWeapon->SetWeaponOwner(this);
	}
	bDead = false;
	Health = MaxHealth;
}

void ATG_BaseCharacter::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATG_BaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

ATG_Gun* ATG_BaseCharacter::GetCurrentWeapon() const
{
	return CurrentWeapon;
}

bool ATG_BaseCharacter::GetIsAiming() const
{
	return bAim;
}

bool ATG_BaseCharacter::GetIsDead() const
{
	return bDead;
}

void ATG_BaseCharacter::SetIsAiming(const bool bNewAim)
{
	bAim = bNewAim;
}

void ATG_BaseCharacter::OnTakeAnyDamageHandle(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	ReceiveDamage(Damage);
}

void ATG_BaseCharacter::ReceiveDamage(const float Damage)
{
	if (bDead) return;

	if (Health - Damage > 0)
	{
		Health -= Damage;
	}
	else
	{
		Health = 0;
		CharacterDying();
	}
}

float ATG_BaseCharacter::GetHealth() const
{
	return Health;
}

float ATG_BaseCharacter::GetMaxHealth() const
{
	return MaxHealth;
}

void ATG_BaseCharacter::SetHealth(const float NewHealth)
{
	Health = NewHealth;
}

void ATG_BaseCharacter::CharacterDying()
{
	bDead = true;
	bAim = false;
	GetCharacterMovement()->DisableMovement();
	if (CurrentWeapon)
	{
		CurrentWeapon->ReleaseTrigger();
	}	
	//TODO: Ragdoll simulation ? 
	SetLifeSpan(5.f);
}
