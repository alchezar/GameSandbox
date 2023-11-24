// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P12/Public/Util/P12CoreTypes.h"
#include "P12WeaponWheelWidget.generated.h"

struct FP12WeaponTableRow;
class UP12EquipmentComponent;
class UTextBlock;
class UImage;

UCLASS()
class GAMESANDBOX_API UP12WeaponWheelWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitializeWheelWidget(UP12EquipmentComponent* InEquipmentComp);
	void NextSegment();
	void PrevSegment();
	void ConfirmSelection();

protected:
	virtual void NativeConstruct() override;
	void SelectSegment();

private:
	FP12WeaponTableRow* GetTableRowForSegment(int32 SegmentIndex) const;

protected:
	UPROPERTY(meta = (BindWidget))
	UImage* RadialBackground;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* WeaponNameText;

	/* Array to match between equipment slots and radial menu segments (array indexes). */	 
	UPROPERTY(EditDefaultsOnly, Category = "C++")
	TArray<EP12EquipmentSlot> EquipmentSlotsSegments;

private:
	int32 CurrentSegmentIndex;
	UPROPERTY()
	UMaterialInstanceDynamic* BackgroundMaterial;
	TWeakObjectPtr<UP12EquipmentComponent> EquipmentComponent;
};
