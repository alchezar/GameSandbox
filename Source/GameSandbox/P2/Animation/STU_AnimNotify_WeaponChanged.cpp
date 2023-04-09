// Copyright (C) 2023, IKinder

#include "STU_AnimNotify_WeaponChanged.h"

void USTU_AnimNotify_WeaponChanged::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	OnChangedNotified.Broadcast(MeshComp);
	Super::Notify(MeshComp, Animation, EventReference);
}
