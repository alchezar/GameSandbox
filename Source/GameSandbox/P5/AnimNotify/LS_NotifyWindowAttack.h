// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "AnimNotifies/AnimNotify_PlayMontageNotify.h"
#include "LS_NotifyWindowAttack.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FLSOnAttackBeginSignature, USkeletalMeshComponent*);
DECLARE_MULTICAST_DELEGATE_OneParam(FLSOnAttackEndSignature, USkeletalMeshComponent*);

UCLASS()
class GAMESANDBOX_API ULS_NotifyWindowAttack : public UAnimNotify_PlayMontageNotifyWindow
{
	GENERATED_BODY()

public:
	ULS_NotifyWindowAttack();
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	FLSOnAttackBeginSignature LSOnAttackBegin;
	FLSOnAttackEndSignature LSOnAttackEnd;
};
