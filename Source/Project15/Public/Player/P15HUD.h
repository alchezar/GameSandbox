// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "P15HUD.generated.h"

class UP15PrimaryWidget;

UCLASS()
class PROJECT15_API AP15HUD : public AHUD
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
protected:
	virtual void BeginPlay() override;

	/* ------------------------------ Fields ------------------------------- */
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TSubclassOf<UP15PrimaryWidget> PrimaryWidgetClass = nullptr;

private:
	TObjectPtr<UP15PrimaryWidget> PrimaryWidget = nullptr;
};
