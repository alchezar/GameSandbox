// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "STU_AnimNotify_ReloadFinished.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnReloadedNotifiedSignature, USkeletalMeshComponent*);

UCLASS()
class GAMESANDBOX_API USTU_AnimNotify_ReloadFinished : public UAnimNotify
{
	GENERATED_BODY()

public:
	FOnReloadedNotifiedSignature OnReloadedNotified;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
