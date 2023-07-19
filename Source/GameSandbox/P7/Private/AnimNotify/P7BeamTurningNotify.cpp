// Copyright (C) 2023, IKinder

#include "P7/Public/AnimNotify/P7BeamTurningNotify.h"

void UP7BeamTurningNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	OnBeamTurning.Broadcast(MeshComp);
}
