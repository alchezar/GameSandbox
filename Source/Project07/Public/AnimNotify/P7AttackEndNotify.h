// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "P7AttackEndNotify.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FP7OnAnimEndSignature, USkeletalMeshComponent*)

UCLASS()
class PROJECT07_API UP7AttackEndNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	FP7OnAnimEndSignature OnAnimEnd;
};
