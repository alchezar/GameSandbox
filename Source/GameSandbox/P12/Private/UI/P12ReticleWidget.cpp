// Copyright (C) 2023, IKinder

#include "P12/Public/UI/P12ReticleWidget.h"

#include "P12/Public/Player/P12BaseCharacter.h"

void UP12ReticleWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CachedCharacter = GetOwningPlayerPawn<AP12BaseCharacter>();
	if (!CachedCharacter.IsValid())
	{
		return;
	}
	CachedCharacter->OnAimingStateChanged.AddUObject(this, &ThisClass::OnAimStateChangedHandle);
}

void UP12ReticleWidget::OnAimStateChangedHandle(const bool bStart)
{
	PlayAnimation(ReticleAiming, 0.f, 1, bStart ? EUMGSequencePlayMode::Forward : EUMGSequencePlayMode::Reverse);
}
