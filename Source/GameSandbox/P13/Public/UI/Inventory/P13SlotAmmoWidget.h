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
	void InitSlot(const FP13AmmoSlot NewAmmoSlot);
	void OnWeaponChangedHandle(FName WeaponID, const FP13WeaponDynamicInfo* DynamicInfo, int32 WeaponIndex);
	void OnAmmoChangedHandle(const EP13WeaponType CurrentWeaponType, const int32 InWeaponNewCount, const int32 InInventoryNewCount) const;

private:
	void TryCacheGameInstance();
	void UpdateAmmoCount(const int32 NewCount) const;
	void UpdateAmmoUsageStatus(const EP13WeaponType TypeToCompare) const;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                               Variables                               *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
protected:
	UPROPERTY(EditAnywhere, Category = "C++")
	TMap<EP13WeaponType, UTexture2D*> AmmoIcons;

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
	int32 MaxAmmoCount = 0;
	EP13WeaponType WeaponType;
	TSoftObjectPtr<UP13GameInstance> GameInstanceCached;
};
