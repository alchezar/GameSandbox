// Copyright (C) 2023, IKinder

#include "LS_NotifySaberVisibility.h"

void ULS_NotifySaberVisibility::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	LSOnSaberVisibility.Broadcast(MeshComp);
	Super::Notify(MeshComp, Animation, EventReference);
}
