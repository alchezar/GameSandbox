// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "TG_AN_Fire.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGunFiredSignature, USkeletalMeshComponent*);

UCLASS()
class PROJECT04_API UTG_AN_Fire : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	FOnGunFiredSignature OnGunFired;
};
