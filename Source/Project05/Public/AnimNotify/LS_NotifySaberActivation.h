// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "LS_NotifySaberActivation.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FLSOnSaberActivationSignature, USkeletalMeshComponent*);

UCLASS()
class PROJECT05_API ULS_NotifySaberActivation : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	FLSOnSaberActivationSignature LSOnSaberActivation;
};
