// Copyright (C) 2023, IKinder

#include "LS_NotifySaberActivation.h"

void ULS_NotifySaberActivation::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	LSOnSaberActivation.Broadcast(MeshComp);
	Super::Notify(MeshComp, Animation, EventReference);
}
