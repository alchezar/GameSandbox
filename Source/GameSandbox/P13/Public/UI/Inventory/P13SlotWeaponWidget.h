// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P13/Public/Library/P13Types.h"
#include "P13SlotWeaponWidget.generated.h"

class UP13GameInstance;
class UBorder;
class UImage;
class UTextBlock;
struct FP13WeaponSlot;

UCLASS()
class GAMESANDBOX_API UP13SlotWeaponWidget : public UUserWidget
{
	GENERATED_BODY()

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                                Super                                  *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
public:
	virtual void NativeConstruct() override;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                                 This                                  *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
public:
	void InitSlot(const int32 NewWeaponIndex, const FP13WeaponSlot& NewWeaponSlot);
	void OnWeaponChangedHandle(const FP13WeaponSlot& NewWeaponSlot, const int32 WeaponIndex);
	void OnAmmoChangedHandle(const EP13AmmoType CurrentWeaponType, const int32 InWeaponNewCount, const int32 InInventoryNewCount) const;

private:
	void TryCacheGameInstance();
	void UpdateWeaponUsageStatus(const int32 IndexToCompare = 0) const;
	void UpdateAmmoCount(const int32 NewCount) const;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                               Variables                               *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* WeaponIndexText = nullptr;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* AmmoCountText = nullptr;
	UPROPERTY(meta = (BindWidget))
	UImage* WeaponImage = nullptr;
	UPROPERTY(meta = (BindWidget))
	UBorder* BackgroundBorder;

	UPROPERTY(EditAnywhere, Category = "C++")
	FLinearColor ActiveBorderColor = {0.f, 0.f, 0.f, 0.75f};
	UPROPERTY(EditAnywhere, Category = "C++")
	FLinearColor InactiveBorderColor = {0.f, 0.f, 0.f, 0.25f};

private:
	int32 CurrentWeaponIndex = 0;
	int32 MagazineCapacity = 0;
	TSoftObjectPtr<UP13GameInstance> GameInstanceCached;
	EP13AmmoType AmmoType = EP13AmmoType::Default;
};
