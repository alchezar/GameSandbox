// Copyright (C) 2023, IKinder

#include "TG_AN_Fire.h"

void UTG_AN_Fire::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	OnGunFired.Broadcast(MeshComp);	
	Super::Notify(MeshComp, Animation, EventReference);
}
