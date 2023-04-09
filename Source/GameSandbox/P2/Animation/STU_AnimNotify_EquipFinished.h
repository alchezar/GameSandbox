// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "STU_AnimNotify_EquipFinished.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnFinishedNotifiedSignature, USkeletalMeshComponent*);

UCLASS()
class GAMESANDBOX_API USTU_AnimNotify_EquipFinished : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	FOnFinishedNotifiedSignature OnFinishedNotified;
};
