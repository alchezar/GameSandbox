// Copyright (C) 2023, IKinder

#include "UI/Equipment/P12WeaponWheelWidget.h"

#include "Actor/Equipment/P12EquipableItem.h"
#include "Blueprint/WidgetTree.h"
#include "Component/Actor/P12EquipmentComponent.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Inventory/P12InventoryItem.h"
#include "Util/P12DataTableUtil.h"

void UP12WeaponWheelWidget::InitializeWheelWidget(UP12EquipmentComponent* InEquipmentComp)
{
	EquipmentComponent = InEquipmentComp;
}

void UP12WeaponWheelWidget::NextSegment()
{
	CurrentSegmentIndex = ++CurrentSegmentIndex % EquipmentSlotsSegments.Num();
	SelectSegment();
}

void UP12WeaponWheelWidget::PrevSegment()
{
	if (--CurrentSegmentIndex < 0)
	{
		CurrentSegmentIndex += EquipmentSlotsSegments.Num();
	}
	SelectSegment();
}

void UP12WeaponWheelWidget::ConfirmSelection()
{
	const EP12EquipmentSlot SelectedSlot = EquipmentSlotsSegments[CurrentSegmentIndex];
	EquipmentComponent->EquipItemInSlot(SelectedSlot);
	RemoveFromParent();
}

void UP12WeaponWheelWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!RadialBackground || BackgroundMaterial)
	{
		return;
	}
	BackgroundMaterial = RadialBackground->GetDynamicMaterial();
	check(BackgroundMaterial)
	BackgroundMaterial->SetScalarParameterValue("Segments", EquipmentSlotsSegments.Num());

	for (int32 Index = 0; Index < EquipmentSlotsSegments.Num(); ++Index)
	{
		FName WidgetName = *("ImageSegment" + FString::FormatAsNumber(Index));
		UImage* WeaponImage = WidgetTree->FindWidget<UImage>(WidgetName);
		if (!WeaponImage)
		{
			continue;
		}
		const FP12WeaponTableRow* WeaponRow = GetTableRowForSegment(Index);
		if (!WeaponRow)
		{
			WeaponImage->SetOpacity(0.5f);
			continue;
		}
		WeaponImage->SetBrushFromTexture(WeaponRow->WeaponItemDescription.Icon);
	}
}

void UP12WeaponWheelWidget::SelectSegment()
{
	BackgroundMaterial->SetScalarParameterValue("Index", CurrentSegmentIndex);
	const FP12WeaponTableRow* WeaponRow = GetTableRowForSegment(CurrentSegmentIndex);
	if (!WeaponRow)
	{
		WeaponNameText->SetVisibility(ESlateVisibility::Collapsed);
	}
	WeaponNameText->SetVisibility(ESlateVisibility::Visible);
	WeaponNameText->SetText(WeaponRow->WeaponItemDescription.Name);
}

FP12WeaponTableRow* UP12WeaponWheelWidget::GetTableRowForSegment(int32 SegmentIndex) const
{
	const EP12EquipmentSlot& SegmentSlot = EquipmentSlotsSegments[SegmentIndex];

	const AP12EquipableItem* EquipableItem = EquipmentComponent->GetItems()[static_cast<int32>(SegmentSlot)];
	if (!EquipableItem)
	{
		return nullptr;
	}
	const FName WeaponID = EquipableItem->GetDataTableID();

	return P12DataTableUtil::FindWeaponData(WeaponID);
}
