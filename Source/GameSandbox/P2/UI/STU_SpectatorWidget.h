// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STU_SpectatorWidget.generated.h"

UCLASS()
class GAMESANDBOX_API USTU_SpectatorWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Kinder")
	bool GetRespawnTime(int32& CountDownTime) const;
};
