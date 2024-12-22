// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimMontage.h"
#include "P7BeamTurningNotify.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FP7OnBeamTurningSignature, USkeletalMeshComponent*)

UCLASS()
class GAMESANDBOX_API UP7BeamTurningNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	FP7OnBeamTurningSignature OnBeamTurning;
};
