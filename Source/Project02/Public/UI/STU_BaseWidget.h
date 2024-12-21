// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STU_BaseWidget.generated.h"

class USoundCue;

UCLASS

()
class PROJECT02_API USTU_BaseWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void PlayShow();
	
protected:
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* ShowAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Sound")
	USoundCue* OpenSound;
};
