// Copyright (C) 2023, IKinder

#include "STU_BaseWidget.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

void USTU_BaseWidget::PlayShow()
{
	PlayAnimation(ShowAnimation);
	UGameplayStatics::PlaySound2D(GetWorld(), OpenSound);
}
