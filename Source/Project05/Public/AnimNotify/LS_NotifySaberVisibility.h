// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "LS_NotifySaberVisibility.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FLSOnSaberVisibilitySignature, USkeletalMeshComponent*)

UCLASS()
class PROJECT05_API ULS_NotifySaberVisibility : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	FLSOnSaberVisibilitySignature LSOnSaberVisibility;
};
