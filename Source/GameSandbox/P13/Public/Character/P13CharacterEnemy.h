// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "P13CharacterBase.h"
#include "P13CharacterEnemy.generated.h"

class UP13ScoreComponent;

UCLASS()
class GAMESANDBOX_API AP13CharacterEnemy : public AP13CharacterBase
{
	GENERATED_BODY()

	/* ------------------------------- Super ------------------------------- */
public:
	AP13CharacterEnemy();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	/* ------------------------------- This -------------------------------- */
public:
	UFUNCTION(BlueprintCallable)
	void EnemyFireAttempt(const FVector& TargetLocation);

protected:
	virtual void OnDeathHandle(AController* Causer) override;
	virtual void InitWeapon(const FP13WeaponSlot& NewWeaponSlot, const int32 CurrentIndex) override;
	virtual void OnWeaponReloadFinishHandle(const int32 RoundNum, const int32 WeaponIndex, const bool bSuccess) override;
	void OnNewWeaponTakenHandle(const int32 NewWeaponIndex, const FP13WeaponSlot& NewWeaponSlot);
	void OnAmmoChangedHandle(const EP13AmmoType InCurrentWeaponType, const int32 InWeaponNewCount, const int32 InInventoryNewCount);
	virtual void UpdateInventoryAfterRespawn() override;

private:
	void CreateEnemyComponents();
	void MoveToPlayer() const;

	/* ----------------------------- Variables ----------------------------- */
protected:
	UPROPERTY(EditAnywhere, Category = "C++")
	UP13ScoreComponent* ScoreComp;

	UPROPERTY(EditAnywhere, Category = "C++")
	float AcceptanceRadius = 200.f;

private:
	bool bReloading = false;
	bool bNextWeaponAvailable = true;
};
