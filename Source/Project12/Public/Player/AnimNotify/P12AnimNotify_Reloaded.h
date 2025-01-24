// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "P12AnimNotify_Reloaded.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FP12AnimNotifyOnReloadedSignature, USkeletalMeshComponent*)

UCLASS()
class PROJECT12_API UP12AnimNotify_Reloaded : public UAnimNotify
{
	GENERATED_BODY()

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	FP12AnimNotifyOnReloadedSignature OnFullReloaded;
};
