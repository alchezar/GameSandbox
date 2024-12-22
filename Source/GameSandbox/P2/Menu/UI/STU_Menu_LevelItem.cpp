// Copyright (C) 2023, IKinder

#include "P2/Menu/UI/STU_Menu_LevelItem.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void USTU_Menu_LevelItem::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (LevelSelectButton)
	{
		LevelSelectButton->OnClicked.AddDynamic(this, &ThisClass::OnLevelItemClicked);
		LevelSelectButton->OnHovered.AddDynamic(this, &ThisClass::OnLevelItemHovered);
		LevelSelectButton->OnUnhovered.AddDynamic(this, &ThisClass::OnLevelItemUnHovered);
	}
}

void USTU_Menu_LevelItem::OnLevelItemClicked()
{
	OnLevelSelected.Broadcast(LevelData);
}

FLevelData USTU_Menu_LevelItem::GetLevelData() const
{
	return LevelData;
}

void USTU_Menu_LevelItem::SetLevelData(const FLevelData& Data)
{
	LevelData = Data;
	if (LevelNameTextBlock)
	{
		LevelNameTextBlock->SetText(FText::FromName(Data.LevelDisplayName));
	}
	if (LevelImage)
	{
		LevelImage->SetBrushFromTexture(Data.LevelThump);
	}
}

void USTU_Menu_LevelItem::SetSelected(const bool bSelected)
{
	if (LevelImage)
	{
		LevelImage->SetColorAndOpacity(bSelected ? FLinearColor::Yellow : FLinearColor::White);
	}
}

void USTU_Menu_LevelItem::OnLevelItemHovered()
{
	if (FrameImage)
	{
		FrameImage->SetVisibility(ESlateVisibility::Visible);
	}
}

void USTU_Menu_LevelItem::OnLevelItemUnHovered()
{
	if (FrameImage)
	{
		FrameImage->SetVisibility(ESlateVisibility::Hidden);
	}
}
