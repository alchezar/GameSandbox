// Copyright (C) 2023, IKinder

#include "P7/Public/AnimNotify/P7HiltVisibilityNotify.h"

void UP7HiltVisibilityNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	OnHiltVisibility.Broadcast(MeshComp);
}
