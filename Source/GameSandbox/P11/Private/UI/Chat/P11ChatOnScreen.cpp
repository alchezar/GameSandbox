// Copyright (C) 2023, IKinder

#include "P11/Public/UI/Chat/P11ChatOnScreen.h"

#include "P11/Public/UI/Chat/P11ChatBox.h"

void UP11ChatOnScreen::NativeConstruct()
{
	Super::NativeConstruct();
}

void UP11ChatOnScreen::ExtendChat(const bool bExtended) const
{
	ChatBox->ExtendChat(bExtended);
}
