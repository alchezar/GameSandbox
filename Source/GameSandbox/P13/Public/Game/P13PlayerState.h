// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "P13/Public/Library/P13Types.h"
#include "P13PlayerState.generated.h"

struct FP13AmmoSlot;
struct FP13WeaponSlot;

DECLARE_MULTICAST_DELEGATE(FP13OnPlyaerStateInitSignature)
DECLARE_MULTICAST_DELEGATE_OneParam(FP13OnScoreChangedSignature, int32 /*Score*/)
DECLARE_MULTICAST_DELEGATE_OneParam(FP13OnLivesChangedSignature, int32 /*Lives*/)

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
	FORCEINLINE int32 GetTotalLives() const { return TotalLives; }
	FORCEINLINE int32 GetTotalScore() const { return TotalScore; }
	FORCEINLINE int32 GetTotalTime() const { return TotalTime; }
	bool GetCanLoadPlayerInventory() const;
	bool GetPlayerCanRespawnAfterDeath();
	bool GetPlayerCanRespawn() const;
	void OnPlayerDied();

	TArray<FP13WeaponSlot> GetSavedWeaponSlots();
	TArray<FP13AmmoSlot> GetSavedAmmoSlots();
	void SavePlayerInventory(const TArray<FP13WeaponSlot>& WeaponSlots, const TArray<FP13AmmoSlot>& AmmoSlots);
	void AddScore(const int32 NewScore);
	void SaveTotalTime(const int32 LastLifeTime);

private:
	void ResetLives();

	/* ----------------------------- Variables ----------------------------- */
public:
	FP13OnPlyaerStateInitSignature OnPlayerStateInit;
	FP13OnScoreChangedSignature OnScoreChanged;
	FP13OnLivesChangedSignature OnLivesChanged;
	
protected:
	UPROPERTY(EditAnywhere, Category = "C++")
	int32 MaxLives = 3;

private:
	int32 TotalLives = 0;
	int32 TotalScore = 0;
	int32 TotalTime = 0;
	TArray<FP13WeaponSlot> LastLifeWeaponSlots = {};
	TArray<FP13AmmoSlot> LastLifeAmmoSlots = {};
};
