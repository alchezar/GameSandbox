// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P13InGameWidget.generated.h"

class UP13HealthWidget;
class UP13InventoryStatsWidget;

UCLASS()
class GAMESANDBOX_API UP13InGameWidget : public UUserWidget
{
	GENERATED_BODY()

	/* ------------------------------- Super ------------------------------- */
protected:
	virtual void NativeConstruct() override;

	/* ------------------------------- This -------------------------------- */
private:
	void ShowStatWidgets() const;
	void ShowHealthWidget() const;
	UFUNCTION()
	void OnPawnChangedHandle(APawn* OldPawn, APawn* NewPawn);

	/* ----------------------------- Variables ----------------------------- */
protected:
	UPROPERTY(EditAnywhere, Category = "C++")
	TSubclassOf<UP13InventoryStatsWidget> InventoryStatClass;
	UPROPERTY(EditAnywhere, Category = "C++")
	TSubclassOf<UP13HealthWidget> HealthClass;
};
