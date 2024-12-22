// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "P7BeltSnappingNotify.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FP7OnBeltSnappingSignature, USkeletalMeshComponent*)

UCLASS()
class PROJECT07_API UP7BeltSnappingNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	FP7OnBeltSnappingSignature OnBeltSnapping;
};
