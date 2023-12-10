// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "AnimNotifies/AnimNotify_PlayMontageNotify.h"
#include "P12AnimNotifyWindow_Equipping.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FP12OnEquippingStartedSignature, USkeletalMeshComponent*)
DECLARE_MULTICAST_DELEGATE_OneParam(FP12OnEquippingFinishedSignature, USkeletalMeshComponent*)

UCLASS()
class GAMESANDBOX_API UP12AnimNotifyWindow_Equipping : public UAnimNotify_PlayMontageNotifyWindow
{
	GENERATED_BODY()

protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	FP12OnEquippingStartedSignature OnEquippingStarted;
	FP12OnEquippingFinishedSignature OnEquippingFinished;
};
