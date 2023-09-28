// Copyright (C) 2023, IKinder

#include "P11/Public/UI/ServerList/P11ServerList.h"

#include "Components/Button.h"

void UP11ServerList::NativeConstruct()
{
	Super::NativeConstruct();

	if (!BackBtn || !RefreshBtn || !ServerListScrl)
	{
		return;
	}
	BackBtn->OnClicked.AddDynamic(this, &ThisClass::OnBackHandle);
	RefreshBtn->OnClicked.AddDynamic(this, &ThisClass::OnRefreshHandle);
}

void UP11ServerList::OnBackHandle()
{
	OnBackServerListBackPressed.Broadcast();	
}

void UP11ServerList::OnRefreshHandle()
{
	
}
