// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotifies/AnimNotify_PlayMontageNotify.h"
#include "P12AnimNotify_Thrown.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FP12OnThrowableTakenSingature, USkeletalMeshComponent*);
DECLARE_MULTICAST_DELEGATE_OneParam(FP12OnThrowableLaunchedSignature, USkeletalMeshComponent*);

UCLASS()
class GAMESANDBOX_API UP12AnimNotify_Thrown : public UAnimNotify_PlayMontageNotifyWindow
{
	GENERATED_BODY()

protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	FP12OnThrowableTakenSingature OnThrowableTaken;
	FP12OnThrowableLaunchedSignature OnThrowableLaunched;
};
