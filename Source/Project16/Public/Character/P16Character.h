// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P16CharacterBase.h"
#include "P16Character.generated.h"

UCLASS()
class PROJECT16_API AP16Character : public AP16CharacterBase
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
public:
	AP16Character();
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(const float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	/* ------------------------------ Fields ------------------------------- */
protected:
};
