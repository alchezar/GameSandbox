// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "AnimNotifies/AnimNotify_PlayMontageNotify.h"
#include "P9NotifyWindowPunch.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FP9OnPunchStartSignature, USkeletalMeshComponent*/* MeshComp */)
DECLARE_MULTICAST_DELEGATE_OneParam(FP9OnPunchEndSignature, USkeletalMeshComponent* /* MeshComp */)

UCLASS()
class PROJECT09_API UP9NotifyWindowPunch : public UAnimNotify_PlayMontageNotifyWindow
{
	GENERATED_BODY()

public:
	UP9NotifyWindowPunch();
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	FP9OnPunchStartSignature OnPunchStart;
	FP9OnPunchEndSignature OnPunchEnd;
};
