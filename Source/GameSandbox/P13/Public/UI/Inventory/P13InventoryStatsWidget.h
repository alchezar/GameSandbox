// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P13InventoryStatsWidget.generated.h"

class AP13Weapon;
class UP13InventoryComponent;
class UP13GameInstance;
class UHorizontalBox;
class UP13SlotAmmoWidget;
class UP13SlotWeaponWidget;
struct FP13WeaponSlot;
struct FP13AmmoSlot;

UCLASS()
class GAMESANDBOX_API UP13InventoryStatsWidget : public UUserWidget
{
	GENERATED_BODY()

	/* ------------------------------- Super ------------------------------- */
protected:
	virtual void NativeConstruct() override;

	/* ------------------------------- This -------------------------------- */
protected:
	void ShowAllWeapons() const;
	void ShowAllAmmo() const;
	void OnNewWeaponTakenHandle(const int32 NewWeaponIndex, const FP13WeaponSlot& NewWeaponSlot);
	void OnNewAmmoTakenHandle(const FP13AmmoSlot& NewAmmoSlot);
	void CacheInventoryComponent();

private:
	void ShowStatWidgets();
	void AddNewWeaponSlotWidget(const int32 NewWeaponIndex, const FP13WeaponSlot& NewWeaponSlot) const;
	void AddNewAmmoSlotWidget(const FP13AmmoSlot& NewAmmoSlot) const;
	void OnWeaponSwitchFinishHandle(const int32 WeaponIndex, AP13Weapon* NewWeapon);

	/* ----------------------------- Variables ----------------------------- */
protected:
	UPROPERTY(EditAnywhere, Category = "C++")
	TSubclassOf<UP13SlotWeaponWidget> WeaponSlotClass;
	UPROPERTY(EditAnywhere, Category = "C++")
	TSubclassOf<UP13SlotAmmoWidget> AmmoSlotClass;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* WeaponBox;
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* AmmoBox;

private:
	TSoftObjectPtr<UP13GameInstance> GameInstanceCached;
	TWeakObjectPtr<UP13InventoryComponent> InventoryComponentCached;
};
