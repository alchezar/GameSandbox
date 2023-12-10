// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P12/Public/Player/AnimInstance/P12BaseCharacterAnimInstance.h"
#include "P12EquipmentViewWidget.generated.h"

class UP12EquipmentSlotWidget;
class UVerticalBox;
class AP12EquipableItem;

UCLASS()
class GAMESANDBOX_API UP12EquipmentViewWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitializeViewWidget(UP12EquipmentComponent* InEquipmentComponent);

protected:
	void AddEquipmentSlotView(AP12EquipableItem* LinkToWeapon, const int32 SlotIndex);
	void UpdateSlot(int32 SlotIndex);
	bool EquipEquipmentToSlot(const TSubclassOf<AP12EquipableItem>& WeaponClass, const int32 SenderIndex);
	void RemoveEquipmentFromSlot(int32 SlotIndex);

protected:
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* VerticalBoxWeaponSlots;
	UPROPERTY(EditDefaultsOnly, Category = "C++")
	TSubclassOf<UP12EquipmentSlotWidget> DefaultSlotWidgetClass;

private:
	TWeakObjectPtr<UP12EquipmentComponent> LinkedEquipmentComponent;
};
