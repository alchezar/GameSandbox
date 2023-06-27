// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "P7HiltVisibilityNotify.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FP7OnHiltVisibilitySignature, USkeletalMeshComponent*)

UCLASS()
class GAMESANDBOX_API UP7HiltVisibilityNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	FP7OnHiltVisibilitySignature OnHiltVisibility;
};
