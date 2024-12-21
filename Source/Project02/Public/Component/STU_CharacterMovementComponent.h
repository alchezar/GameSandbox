// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "STU_CharacterMovementComponent.generated.h"

UCLASS()
class PROJECT02_API USTU_CharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	USTU_CharacterMovementComponent();
	virtual float GetMaxSpeed() const override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | Speed", meta = (ClampMin = "1.5", ClampMax = "3.0"))
	float RunModifier = 2.f;
};
