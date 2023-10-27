// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "P12AnimNotify_ReloadOneAmmo.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FP12OnReloadOneAmmoSignature, USkeletalMeshComponent*, int32)

UCLASS()
class GAMESANDBOX_API UP12AnimNotify_ReloadOneAmmo : public UAnimNotify
{
	GENERATED_BODY()

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	FP12OnReloadOneAmmoSignature OnOneReload;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++", meta = (ClampMin = 1, UIMin = 1))
	int32 NumberOfAmmo = 1;
};
