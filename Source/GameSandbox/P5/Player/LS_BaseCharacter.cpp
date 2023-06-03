// Copyright (C) 2023, IKinder

#include "LS_BaseCharacter.h"
#include "GameFramework/PawnMovementComponent.h"
#include "P5/AnimNotify/LS_NotifySaberActivation.h"
#include "P5/AnimNotify/LS_NotifySaberVisibility.h"
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
	SetTeamColor();
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

void ALS_BaseCharacter::SpawnWeapon()
{
	if (!LightSaberClass) return;

	CurrentSaber = GetWorld()->SpawnActor<ALS_LightSaber>(LightSaberClass);
	if (!CurrentSaber) return;

	CurrentSaber->SetSaberOwner(this);
	CurrentSaber->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
	CurrentSaber->SetSaberColor(TeamColor);
	CurrentSaber->SetActorRelativeLocation(FVector(0.f, 2.f, 0.f));
	CurrentSaber->SetActorRelativeRotation(FRotator(25.f, 0.f, 10.f));
}

void ALS_BaseCharacter::SetTeamColor()
{
	UMaterialInstanceDynamic* DynamicBodyMaterial1 = GetMesh()->CreateAndSetMaterialInstanceDynamic(1);
	UMaterialInstanceDynamic* DynamicBodyMaterial2 = GetMesh()->CreateAndSetMaterialInstanceDynamic(2);
	UMaterialInstanceDynamic* DynamicBodyMaterial3 = GetMesh()->CreateAndSetMaterialInstanceDynamic(3);

	if (!DynamicBodyMaterial1 || !DynamicBodyMaterial2 || !DynamicBodyMaterial3) return;

	DynamicBodyMaterial1->SetVectorParameterValue(TeamColorParameterName, TeamColor);
	DynamicBodyMaterial2->SetVectorParameterValue(TeamColorParameterName, TeamColor);
	DynamicBodyMaterial3->SetVectorParameterValue(TeamColorParameterName, TeamColor);
}

void ALS_BaseCharacter::Attack()
{
	if (!bTaken || AttackMontageArray.IsEmpty() || !AttackMontageArray[0] || bAttacking) return;

	bAttacking = true;
	const float MontageLength = PlayAnimMontage(AttackMontageArray[CompoCount++ % AttackMontageArray.Num()]);
	/* Wait till animation end */
	GetWorldTimerManager().ClearTimer(AttackTimer);
	FTimerDelegate AttackDelegate;
	AttackDelegate.BindLambda([&]()
	{
		CompoCount = 0;
	});
	GetWorldTimerManager().SetTimer(AttackTimer, AttackDelegate, 1.f, false, MontageLength);
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

void ALS_BaseCharacter::TakeWeapon()
{
	PlayAnimMontage(bTaken ? TakeLightSaber.Put : TakeLightSaber.Get);
	bTaken = !bTaken;
}

void ALS_BaseCharacter::Landed(const FHitResult& Hit)
{
	bDoubleJump = false;
	Super::Landed(Hit);
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

	TArray<UAnimMontage*> TakeMontageArray;
	TakeMontageArray.Add(TakeLightSaber.Get);
	TakeMontageArray.Add(TakeLightSaber.Put);
	for (const auto TakeMontage : TakeMontageArray)
	{
		if (!TakeMontage) return;

		TArray<FAnimNotifyEvent> AnimNotifyEvents = TakeMontage->Notifies;
		for (auto NotifyEvent : AnimNotifyEvents)
		{
			if (const auto SaberActivation = Cast<ULS_NotifySaberActivation>(NotifyEvent.Notify))
			{
				SaberActivation->LSOnSaberActivation.AddUObject(this, &ThisClass::OnSaberActivationHandle);
			}
			if (const auto SaberVisibility = Cast<ULS_NotifySaberVisibility>(NotifyEvent.Notify))
			{
				SaberVisibility->LSOnSaberVisibility.AddUObject(this, &ThisClass::OnSaberVisibilityHandle);
			}
		}
	}
}

void ALS_BaseCharacter::OnAttackBeginHandle(USkeletalMeshComponent* MeshComp)
{
	if (MeshComp != GetMesh()) return;

	if (CurrentSaber->GetIsTurnedOn())
	{
		CurrentSaber->EnableRibbon();
	}
}

void ALS_BaseCharacter::OnAttackEndHandle(USkeletalMeshComponent* MeshComp)
{
	if (MeshComp != GetMesh()) return;

	bAttacking = false;
	CurrentSaber->DisableRibbon();
}

void ALS_BaseCharacter::OnSaberActivationHandle(USkeletalMeshComponent* MeshComp)
{
	if (MeshComp != GetMesh()) return;

	bTaken ? GetCurrentSaber()->TurnBeamOn() : GetCurrentSaber()->TurnBeamOff();
}

void ALS_BaseCharacter::OnSaberVisibilityHandle(USkeletalMeshComponent* MeshComp)
{
	if (MeshComp != GetMesh()) return;

	CurrentSaber->SetActorHiddenInGame(!bTaken);
}

bool ALS_BaseCharacter::GetIsAttacking() const
{
	return bAttacking;
}

bool ALS_BaseCharacter::GetIsDead()
{
	return FMath::IsNearlyZero(Health);
}

bool ALS_BaseCharacter::GetIsDoubleJump() const
{
	return bDoubleJump;
}

bool ALS_BaseCharacter::GetIsWeaponTaken() const
{
	return bTaken;
}

ALS_LightSaber* ALS_BaseCharacter::GetCurrentSaber() const
{
	return CurrentSaber;
}

void ALS_BaseCharacter::StartMoving()
{
	if (!MoveMontageArray[0]) return;

	PlayAnimMontage(MoveMontageArray[0]);
}

void ALS_BaseCharacter::StopMoving()
{
	if (!MoveMontageArray[1]) return;

	PlayAnimMontage(MoveMontageArray[1]);
}
