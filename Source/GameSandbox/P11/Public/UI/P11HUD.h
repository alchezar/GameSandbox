// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "P11HUD.generated.h"

class UP11MainWidget;

UCLASS()
class GAMESANDBOX_API AP11HUD : public AHUD
{
	GENERATED_BODY()

public:
	AP11HUD();
	
protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++")
	TSubclassOf<UP11MainWidget> MainWidgetClass;

private:
	UPROPERTY()
	UP11MainWidget* MainWidget;
};
