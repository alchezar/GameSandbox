// Copyright (C) 2023, IKinder

#include "LS_BaseCharacter.h"
#include "GameFramework/PawnMovementComponent.h"
#include "P5/AnimNotify/LS_NotifyWindowAttack.h"
#include "P5/Weapon/LS_LightSaber.h"

ALS_BaseCharacter::ALS_BaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ALS_BaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	SpawnWeapon();
	InitAnimationNotifyStates();
	Health = MaxHealth;
}

void ALS_BaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALS_BaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

bool ALS_BaseCharacter::GetIsDead()
{
	return FMath::IsNearlyZero(Health);
}

void ALS_BaseCharacter::SpawnWeapon()
{
	if (!LightSaberClass) return;

	CurrentSaber = GetWorld()->SpawnActor<ALS_LightSaber>(LightSaberClass);
	if (!CurrentSaber) return;

	CurrentSaber->SetOwner(this);
	CurrentSaber->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
	CurrentSaber->SetSaberColor(SaberColor);
	CurrentSaber->SetActorRelativeLocation(FVector(0.f, 2.f, 0.f));
	CurrentSaber->SetActorRelativeRotation(FRotator(25.f, 0.f, 10.f));
}

void ALS_BaseCharacter::Attack()
{
	if (AttackMontageArray.IsEmpty() || !AttackMontageArray[0] || bAttacking) return;

	bAttacking = true;

	// const int32 RandomAttackIndex = FMath::RandRange(0, AttackMontageArray.Num() - 1);
	PlayAnimMontage(AttackMontageArray[AttackIndex++ % AttackMontageArray.Num()]);
}

void ALS_BaseCharacter::Jump()
{
	Super::Jump();

	if (GetMovementComponent()->IsFalling() && !bDoubleJump)
	{
		bDoubleJump = true;
		LaunchCharacter(GetVelocity() + FVector(0.f, 0.f, 700.f), true, true);
	}
}

void ALS_BaseCharacter::Landed(const FHitResult& Hit)
{
	bDoubleJump = false;
	Super::Landed(Hit);
}

bool ALS_BaseCharacter::GetIsDoubleJump() const
{
	return bDoubleJump;
}

void ALS_BaseCharacter::InitAnimationNotifyStates()
{
	for (const auto AttackMontage : AttackMontageArray)
	{
		if (!AttackMontage) return;

		TArray<FAnimNotifyEvent> AnimNotifyEvents = AttackMontage->Notifies;
		for (auto NotifyEvent : AnimNotifyEvents)
		{
			if (const auto WindowAttack = Cast<ULS_NotifyWindowAttack>(NotifyEvent.NotifyStateClass))
			{
				WindowAttack->LSOnAttackBegin.AddUObject(this, &ThisClass::OnAttackBeginHandle);
				WindowAttack->LSOnAttackEnd.AddUObject(this, &ThisClass::OnAttackEndHandle);
			}
		}
	}
}

void ALS_BaseCharacter::OnAttackBeginHandle(USkeletalMeshComponent* MeshComp)
{
	if (MeshComp == GetMesh())
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("Start")));
	}
}

void ALS_BaseCharacter::OnAttackEndHandle(USkeletalMeshComponent* MeshComp)
{
	if (MeshComp == GetMesh())
	{
		bAttacking = false;
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("End")));
	}
}

ALS_LightSaber* ALS_BaseCharacter::GetCurrentSaber() const 
{
	return CurrentSaber;
}
