// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Utils/P15Utils.h"
#include "P15AnimNotifies.generated.h"

UCLASS()
class PROJECT15_API UP15AnimAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override
	{
		OnAnimNotify.Broadcast(MeshComp);
		Super::Notify(MeshComp, Animation, EventReference);
	}

public:
	FP15OnAnimNotifySignature OnAnimNotify;
};
