// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "STU_AnimNotify_WeaponChanged.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnChangedNotifiedSignature, USkeletalMeshComponent*);

UCLASS()
class GAMESANDBOX_API USTU_AnimNotify_WeaponChanged : public UAnimNotify
{
	GENERATED_BODY()

public:
	FOnChangedNotifiedSignature OnChangedNotified;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
