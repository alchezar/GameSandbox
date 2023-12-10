// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "AnimNotifies/AnimNotify_PlayMontageNotify.h"
#include "P12AnimNotifyWindow_MeleeAttack.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FP12OnMeleeAttackStartSignature, USkeletalMeshComponent*)
DECLARE_MULTICAST_DELEGATE_OneParam(FP12OnMeleeAttackEndSignature, USkeletalMeshComponent*)

UCLASS()
class GAMESANDBOX_API UP12AnimNotifyWindow_MeleeAttack : public UAnimNotify_PlayMontageNotifyWindow
{
	GENERATED_BODY()

protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	FP12OnMeleeAttackStartSignature OnMeleeAttackStart;
	FP12OnMeleeAttackEndSignature OnMeleeAttackEnd;
};
