// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P13SlotWeaponWidget.generated.h"

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
	void InitSlot(const int32 NewWeaponIndex, UTexture2D* NewWeaponImage);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                               Variables                               *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* WeaponIndex = nullptr;  
	UPROPERTY(meta = (BindWidget))
	UImage* WeaponImage = nullptr;
};
