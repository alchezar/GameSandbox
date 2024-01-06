// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P13/Public/Library/P13Types.h"
#include "P13SlotAmmoWidget.generated.h"

class UP13GameInstance;
class UBorder;
class UImage;
class UTextBlock;
struct FP13AmmoSlot;

UCLASS()
class GAMESANDBOX_API UP13SlotAmmoWidget : public UUserWidget
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
	FORCEINLINE int32 GetAmmoCount() const { return AmmoCount; }
	FORCEINLINE EP13AmmoType GetAmmoType() const { return AmmoType; }
	void InitSlot(const FP13AmmoSlot NewAmmoSlot);
	void OnWeaponChangedHandle(const FP13WeaponSlot& NewWeaponSlot, int32 WeaponIndex);
	void OnAmmoChangedHandle(const EP13AmmoType CurrentWeaponType, const int32 InWeaponNewCount, const int32 InInventoryNewCount);

private:
	void TryCacheGameInstance();
	void UpdateAmmoCount(const int32 NewCount) const;
	void UpdateAmmoUsageStatus(const EP13AmmoType TypeToCompare) const;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                               Variables                               *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
protected:
	UPROPERTY(EditAnywhere, Category = "C++")
	TMap<EP13AmmoType, UTexture2D*> AmmoIcons;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AmmoCountText = nullptr;
	UPROPERTY(meta = (BindWidget))
	UImage* AmmoImage = nullptr;
	UPROPERTY(meta = (BindWidget))
	UBorder* BackgroundBorder;

	UPROPERTY(EditAnywhere, Category = "C++")
	FLinearColor ActiveBorderColor = {0.f, 0.f, 0.f, 0.75f};
	UPROPERTY(EditAnywhere, Category = "C++")
	FLinearColor InactiveBorderColor = {0.f, 0.f, 0.f, 0.25f};

private:
	int32 AmmoCount = 0;
	int32 MaxAmmoCount = 0;
	EP13AmmoType WeaponType;
	TSoftObjectPtr<UP13GameInstance> GameInstanceCached;
	EP13AmmoType AmmoType = EP13AmmoType::Default;
};
