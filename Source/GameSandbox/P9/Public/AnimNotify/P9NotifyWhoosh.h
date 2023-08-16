// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "P9NotifyWhoosh.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FP9OnWhooshSignature, USkeletalMeshComponent* /* MeshComp */)

UCLASS()
class GAMESANDBOX_API UP9NotifyWhoosh : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	FP9OnWhooshSignature OnWhoosh;
};