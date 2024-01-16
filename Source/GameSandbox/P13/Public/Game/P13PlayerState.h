// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "P13PlayerState.generated.h"

struct FP13AmmoSlot;
struct FP13WeaponSlot;

UCLASS()
class GAMESANDBOX_API AP13PlayerState : public APlayerState
{
	GENERATED_BODY()
	
	/* ------------------------------- Super ------------------------------- */
public:
	AP13PlayerState();

protected:
	virtual void BeginPlay() override;
	
	/* ------------------------------- This -------------------------------- */
public:
	bool GetCanLoadPlayerInventory() const;
	bool GetPlayerCanRespawnAfterDeath();
	bool GetPlayerCanRespawn() const;
	void OnPlayerDied();
	
	TArray<FP13WeaponSlot> GetSavedWeaponSlots();
	TArray<FP13AmmoSlot> GetSavedAmmoSlots();
	void SavePlayerInventory(const TArray<FP13WeaponSlot>& WeaponSlots, const TArray<FP13AmmoSlot>& AmmoSlots);

private:
	void ResetLives();

	/* ----------------------------- Variables ----------------------------- */
protected:
	UPROPERTY(EditAnywhere, Category = "C++")
	int32 MaxLives = 3;

private:
	int32 CurrentLives = 0;
	TArray<FP13WeaponSlot> LastLifeWeaponSlots = {};
	TArray<FP13AmmoSlot> LastLifeAmmoSlots = {};
};
