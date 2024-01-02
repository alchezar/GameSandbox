// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P13InGameWidget.generated.h"

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

private:
	void CacheInventoryComponent();
	void ShowStatWidgets();

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
	TWeakObjectPtr<UP13InventoryComponent> InventoryComponentCached;
};
