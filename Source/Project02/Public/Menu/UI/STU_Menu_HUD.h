// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "STU_Menu_HUD.generated.h"

class USTU_MenuWidget;

UCLASS()
class PROJECT02_API ASTU_Menu_HUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | UI")
	TSubclassOf<USTU_MenuWidget> MenuWidgetClass;

};
