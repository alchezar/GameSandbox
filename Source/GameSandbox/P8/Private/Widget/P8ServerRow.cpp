// Copyright (C) 2023, IKinder

#include "P8/Public/Widget/P8ServerRow.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "P8/Public/Widget/P8MainMenuWidget.h"

void UP8ServerRow::NativeConstruct()
{
	Super::NativeConstruct();
	check(RowButton)
	check(ServerName)

	RowButton->OnClicked.AddDynamic(this, &ThisClass::OnRowButtonClickedHandle);
}

void UP8ServerRow::OnRowButtonClickedHandle()
{
	if (!ParentWidget) return;
	ParentWidget->SetSelectedIndex(Index);
}

void UP8ServerRow::SetupRow(UP8MainMenuWidget* Parent, const uint32 NewIndex, const FText& NewName)
{
	ParentWidget = Parent;
	Index = NewIndex;
	ServerName->SetText(NewName);
}
