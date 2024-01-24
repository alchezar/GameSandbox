// Copyright © 2024, IKinder

#include "P13/Public/Character/P13CharacterEnemy.h"

#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "P13/Public/Component/Actor/P13InventoryComponent.h"
#include "P13/Public/Component/Actor/P13ScoreComponent.h"
#include "P13/Public/Game/P13GameMode.h"
#include "P13/Public/Weapon/P13Weapon.h"

AP13CharacterEnemy::AP13CharacterEnemy()
{
	// bUseControllerRotationYaw = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 180.f, 0.f);
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	CreateEnemyComponents();
}

void AP13CharacterEnemy::BeginPlay()
{
	Super::BeginPlay();
	UpdateInventoryAfterRespawn();
}

void AP13CharacterEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// MoveToPlayer();	
}

bool AP13CharacterEnemy::GetCanAttack_Implementation() const
{
	return true;
}

bool AP13CharacterEnemy::GetCanMove_Implementation() const
{
	return true;
}

bool AP13CharacterEnemy::AttackAttempt_Implementation(const FVector& TargetLocation)
{
	return EnemyFireAttempt(TargetLocation);
}

bool AP13CharacterEnemy::EnemyFireAttempt(const FVector& TargetLocation)
{
	if (bReloading || !bNextWeaponAvailable || TargetLocation.IsNearlyZero())
	{
		return false;
	}

	if ((CachedWeapon->GetDynamicInfo().Round > 0))
	{
		const FVector FixedLocation = TargetLocation - FVector(0.f, 0.f, 90.f);
		CachedWeapon->SetTargetLocation(FixedLocation);

		PullTrigger(true);
		PullTrigger(false);
		return true;
	}

	if (TryReloadWeapon())
	{
		bReloading = true;
		return false;
	}

	/* Remember if we can't take next weapon. */
	bNextWeaponAvailable = TryTakeNextWeapon(true);
	return false;
}

void AP13CharacterEnemy::OnDeathHandle(AController* Causer)
{
	Super::OnDeathHandle(Causer);

	if (AP13GameMode* GameMode = GetWorld()->GetAuthGameMode<AP13GameMode>())
	{
		GameMode->RespawnEnemies();
	}
}

void AP13CharacterEnemy::InitWeapon(const FP13WeaponSlot& NewWeaponSlot, const int32 CurrentIndex)
{
	Super::InitWeapon(NewWeaponSlot, CurrentIndex);

	bNextWeaponAvailable = (CachedWeapon->GetDynamicInfo().Round > 0);
}

void AP13CharacterEnemy::OnWeaponReloadFinishHandle(const int32 RoundNum, const int32 WeaponIndex, const bool bSuccess)
{
	Super::OnWeaponReloadFinishHandle(RoundNum, WeaponIndex, bSuccess);
	bReloading = false;
}

void AP13CharacterEnemy::OnNewWeaponTakenHandle(const int32 NewWeaponIndex, const FP13WeaponSlot& NewWeaponSlot)
{
	bNextWeaponAvailable = true;
}

void AP13CharacterEnemy::OnAmmoChangedHandle(const EP13AmmoType InCurrentWeaponType, const int32 InWeaponNewCount, const int32 InInventoryNewCount)
{
	if (InWeaponNewCount < 0)
	{
		bNextWeaponAvailable = true;
	}
}

void AP13CharacterEnemy::UpdateInventoryAfterRespawn()
{
	Super::UpdateInventoryAfterRespawn();

	InventoryComponent->OnNewWeaponTaken.AddUObject(this, &ThisClass::OnNewWeaponTakenHandle);
	InventoryComponent->OnAmmoChanged.AddUObject(this, &ThisClass::OnAmmoChangedHandle);
}

void AP13CharacterEnemy::CreateEnemyComponents()
{
	ScoreComp = CreateDefaultSubobject<UP13ScoreComponent>("EnemyScoreComponent");
}

void AP13CharacterEnemy::MoveToPlayer() const
{
	if (!ControllerCached.IsValid() || !ControllerCached->IsA<AAIController>())
	{
		return;
	}
	AActor* Target = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!Target)
	{
		return;
	}
	const float DistSqr = FVector::DistSquared(GetActorLocation(), Target->GetActorLocation());
	if (DistSqr < FMath::Pow(AcceptanceRadius, 2))
	{
		if (GetVelocity().Size() > 10.f)
		{
			ControllerCached->StopMovement();
		}
		return;
	}
	if (GetVelocity().Size() > 10.f)
	{
		return;
	}
	AAIController* AIController = StaticCast<AAIController*>(ControllerCached.Get());
	AIController->MoveToActor(Target);
}
