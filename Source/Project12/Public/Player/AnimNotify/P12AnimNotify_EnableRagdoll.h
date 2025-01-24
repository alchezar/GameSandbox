// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "P12AnimNotify_EnableRagdoll.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FP12AnimNitifyOnEnableRagdollSignature, USkeletalMeshComponent*)

UCLASS()
class PROJECT12_API UP12AnimNotify_EnableRagdoll : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	FP12AnimNitifyOnEnableRagdollSignature OnEnableRagdoll;
};
