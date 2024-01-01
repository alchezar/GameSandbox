// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P13/Public/Library/P13Types.h"
#include "P13SlotAmmoWidget.generated.h"

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
	void UpdateAmmoCount(const int32 NewCount) const;
	void OnAmmoChangedHandle(const EP13WeaponType CurrentWeaponType, const int32 NewCount) const;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                               Variables                               *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
protected:
	UPROPERTY(EditAnywhere, Category = "C++")
	TMap<EP13WeaponType, UTexture2D*> AmmoIcons;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AmmoCount = nullptr;  
	UPROPERTY(meta = (BindWidget))
	UImage* AmmoImage = nullptr;

private:
	int32 MaxCount = 0;
	EP13WeaponType WeaponType; 
};
