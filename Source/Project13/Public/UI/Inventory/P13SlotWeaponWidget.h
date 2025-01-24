// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Library/P13Types.h"
#include "P13SlotWeaponWidget.generated.h"

class UProgressBar;
class UP13GameInstance;
class UBorder;
class UImage;
class UTextBlock;
struct FP13WeaponSlot;

UCLASS()
class PROJECT13_API UP13SlotWeaponWidget : public UUserWidget
{
	GENERATED_BODY()

	/* ------------------------------- Super ------------------------------- */
public:
	virtual void NativeConstruct() override;

	/* ------------------------------- This -------------------------------- */
public:
	FORCEINLINE EP13AmmoType GetAmmoType() const { return AmmoType; }
	FORCEINLINE int32 GetCurrentWeaponIndex() const { return CurrentWeaponIndex; }
	void InitSlot(const int32 NewWeaponIndex, const FP13WeaponSlot& NewWeaponSlot);
	void OnCurrentWeaponUpdatedHandle(const FP13WeaponSlot& NewWeaponSlot, const int32 WeaponIndex);
	void OnAmmoChangedHandle(const EP13AmmoType CurrentWeaponType, const int32 InWeaponNewCount, const int32 InInventoryNewCount);
	void OnWeaponStartReloadingHandle(UAnimMontage* CharFireMontage, const int32 WeaponIndex, const float ReloadingTime);
	void OnWeaponFinishReloadingHandle(const int32 NewRoundNum, const int32 WeaponIndex, const bool bSuccess);

private:
	void TryCacheGameInstance();
	void UpdateWeaponUsageStatus(const int32 IndexToCompare = 0) const;
	void UpdateAmmoCount(const int32 NewCount);

	/* ----------------------------- Variables ----------------------------- */
protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* WeaponIndexText = nullptr;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* AmmoCountText = nullptr;
	UPROPERTY(meta = (BindWidget))
	UImage* WeaponImage = nullptr;
	UPROPERTY(meta = (BindWidget))
	UBorder* BackgroundBorder;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ReloadingProgressBar;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* ReloadAnim = nullptr;

	UPROPERTY(EditAnywhere, Category = "C++")
	FLinearColor ActiveBorderColor = {0.f, 0.f, 0.f, 0.75f};
	UPROPERTY(EditAnywhere, Category = "C++")
	FLinearColor InactiveBorderColor = {0.f, 0.f, 0.f, 0.25f};

private:
	int32 CurrentWeaponIndex = 0;
	int32 MagazineCapacity = 0;
	int32 MagazineCurrent = 0;
	TSoftObjectPtr<UP13GameInstance> GameInstanceCached;
	EP13AmmoType AmmoType = EP13AmmoType::Default;
	float LastPercentage = 0.f;
};
