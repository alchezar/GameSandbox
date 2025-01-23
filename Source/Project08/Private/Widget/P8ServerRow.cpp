// Copyright (C) 2023, IKinder

#include "Widget/P8ServerRow.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Util/P8Utils.h"
#include "Widget/P8MainMenuWidget.h"

void UP8ServerRow::NativeConstruct()
{
	Super::NativeConstruct();
	check(RowButton)
	check(ServerName)

	RowButton->OnClicked.AddDynamic(this, &ThisClass::OnRowButtonClickedHandle);
	RowButton->OnHovered.AddDynamic(this, &ThisClass::OnRowButtonHoveredHandle);
	RowButton->OnUnhovered.AddDynamic(this, &ThisClass::OnRowButtonUnhoveredHandle);

	AllText.Add(ServerName);
	AllText.Add(ServerOwnerName);
	AllText.Add(Players);
}

void UP8ServerRow::OnRowButtonClickedHandle()
{
	if (!ParentWidget || bHeader) return;
	ParentWidget->SetSelectedIndex(Index);

	bSelected = true;
	SetRowTextColor(FColor::Green);
}

void UP8ServerRow::OnRowButtonHoveredHandle()
{
	if (bHeader) return;
	SetRowTextColor(bSelected ? FColor::Green : FColor::Yellow);
}

void UP8ServerRow::OnRowButtonUnhoveredHandle()
{
	SetRowTextColor(bSelected ? FColor::Green : FColor::White);
}

void UP8ServerRow::SetupRow(UP8MainMenuWidget* Parent, const uint32 NewIndex, const FP8ServerData& Data)
{
	ParentWidget = Parent;
	Index = NewIndex;
	ServerName->SetText(FText::FromString(Data.ServerName.IsEmpty() ? Data.SessionID : Data.ServerName));
	ServerOwnerName->SetText(FText::FromString(Data.HostUserName));
	Players->SetText(FText::FromString(FString::FromInt(Data.CurrentPlayers) + "/" + FString::FromInt(Data.MaxPlayers)));
}

void UP8ServerRow::Unselect()
{
	bSelected = false;
	SetRowTextColor(FColor::White);
}

void UP8ServerRow::SetRowTextColor(const FColor NewColor)
{
	for (UTextBlock* Text : AllText)
	{
		Text->SetColorAndOpacity(FSlateColor(NewColor));
	}
}
