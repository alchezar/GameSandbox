// Copyright (C) 2023, IKinder

#include "P11/Public/UI/ServerList/P11ServerSingle.h"

#include "Components/Button.h"

void UP11ServerSingle::NativeConstruct()
{
	Super::NativeConstruct();

	BackBtn->OnClicked.AddDynamic(this, &ThisClass::OnBackHandle);
	CreateBtn->OnClicked.AddDynamic(this, &ThisClass::OnCreateHandle);
}

void UP11ServerSingle::OnBackHandle()
{
	OnSingleBackBackButtonPressed.Broadcast();
}

void UP11ServerSingle::OnCreateHandle()
{
	
}
