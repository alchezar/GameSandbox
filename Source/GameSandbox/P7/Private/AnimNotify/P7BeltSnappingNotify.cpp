// Copyright (C) 2023, IKinder

#include "P7/Public/AnimNotify/P7BeltSnappingNotify.h"

void UP7BeltSnappingNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	OnBeltSnapping.Broadcast(MeshComp);
}
