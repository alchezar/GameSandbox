// Copyright (C) 2023, IKinder

#include "UI/STU_SpectatorWidget.h"

#include "Component/STU_RespawnComponent.h"

bool USTU_SpectatorWidget::GetRespawnTime(int32& CountDownTime) const
{
	if (!GetOwningPlayer()) return false;

	const USTU_RespawnComponent* RespawnComponent = GetOwningPlayer()->FindComponentByClass<USTU_RespawnComponent>();
	if (!RespawnComponent || !RespawnComponent->IsRespawnInProgress()) return false;

	CountDownTime = RespawnComponent->GetRespawnCountDown();
	return true;	
}
