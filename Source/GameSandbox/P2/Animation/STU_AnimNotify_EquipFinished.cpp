// Copyright (C) 2023, IKinder

#include "STU_AnimNotify_EquipFinished.h"

void USTU_AnimNotify_EquipFinished::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	OnFinishedNotified.Broadcast(MeshComp);
	Super::Notify(MeshComp, Animation, EventReference);
}
