// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P11MainWidget.generated.h"

class UTextBlock;

UCLASS()
class GAMESANDBOX_API UP11MainWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* HealthText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* AmmoText;
};
