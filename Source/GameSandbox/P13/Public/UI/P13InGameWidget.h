// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "P13InGameWidget.generated.h"

struct FP13WeaponSlot;
struct FP13AmmoSlot;
class UP13GameInstance;
class UP13InventoryComponent;
class UHorizontalBox;
class UP13SlotAmmoWidget;
class UP13SlotWeaponWidget;

UCLASS()
class GAMESANDBOX_API UP13InGameWidget : public UUserWidget
{
	GENERATED_BODY()

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                                Super                                  *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
protected:
	virtual void NativeConstruct() override;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                                 This                                  *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
protected:
	void ShowAllWeapons() const;
	void ShowAllAmmo() const;

protected:
	void OnNewWeaponTakenHandle(const int32 NewWeaponIndex, const FP13WeaponSlot& NewWeaponSlot);
	void OnNewAmmoTakenHandle(FP13AmmoSlot NewAmmoSlot);

private:
	void CacheInventoryComponent();
	void ShowStatWidgets();
	void AddNewWeaponSlotWidget(const int32 NewWeaponIndex, const FP13WeaponSlot& NewWeaponSlot) const;
	void AddNewAmmoSlotWidget(const FP13AmmoSlot& NewAmmoSlot) const;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                               Variables                               *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
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
