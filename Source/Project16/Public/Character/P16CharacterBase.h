// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "P16CharacterBase.generated.h"

UCLASS(Abstract)
class PROJECT16_API AP16CharacterBase : public ACharacter
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
public:
	AP16CharacterBase();

protected:
	virtual void BeginPlay() override;

	/* ------------------------------ Fields ------------------------------- */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Component")
	TObjectPtr<USkeletalMeshComponent> Weapon = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Combat")
	FName HandSocketName = "P16_Hand";
};
