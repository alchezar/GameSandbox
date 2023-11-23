// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P12/Public/Player/AnimInstance/P12BaseCharacterAnimInstance.h"
#include "P12EquipmentViewWidget.generated.h"

UCLASS()
class GAMESANDBOX_API UP12EquipmentViewWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitializeViewWidget(UP12EquipmentComponent* UP12EquipmentComponent);
};
