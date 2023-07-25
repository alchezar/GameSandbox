// Copyright (C) 2023, IKinder

#include "P8/Public/Widget/P8ServerRow.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "P8/Public/Widget/P8MainMenuWidget.h"

void UP8ServerRow::NativeConstruct()
{
	Super::NativeConstruct();
	check(RowButton);

	RowButton->OnClicked.AddDynamic(this, &ThisClass::OnRowButtonClickedHandle);
}

void UP8ServerRow::OnRowButtonClickedHandle()
{
	if (!ParentWidget) return;
	ParentWidget->SetSelectedIndex(Index);
}

void UP8ServerRow::Setup(UP8MainMenuWidget* Parent, const uint32 NewIndex)
{
	ParentWidget = Parent;
	Index = NewIndex;
}

void UP8ServerRow::SetServerName(const FText& NewName) const
{
	if (!ServerName) return;
	ServerName->SetText(NewName);
}
