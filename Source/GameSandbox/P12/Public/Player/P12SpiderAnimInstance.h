// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "P12SpiderAnimInstance.generated.h"

class AP12SpiderPawn;

UCLASS()
class GAMESANDBOX_API UP12SpiderAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(BlueprintReadOnly, Transient, Category = "C++")
	float InputForward;
	UPROPERTY(BlueprintReadOnly, Transient, Category = "C++")
	float InputRight;
	UPROPERTY(BlueprintReadOnly, Transient, Category = "C++")
	bool bInAir;

private:
	TWeakObjectPtr<AP12SpiderPawn> CachedPawn;

};
