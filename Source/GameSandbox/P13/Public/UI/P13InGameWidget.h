// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P13InGameWidget.generated.h"

class UP13PlayerStatsWidget;
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
public:
	void HideInGameWidgets();
	void RemoveInGameWidget();

private:
	void ShowInventoryStatWidget();
	void ShowHealthWidget();
	void ShowPlayerStatWidget();

	/* ----------------------------- Variables ----------------------------- */
protected:
	UPROPERTY(EditAnywhere, Category = "C++")
	TSubclassOf<UP13InventoryStatsWidget> InventoryStatClass;
	UPROPERTY(EditAnywhere, Category = "C++")
	TSubclassOf<UP13HealthWidget> HealthClass;
	UPROPERTY(EditAnywhere, Category = "C++")
	TSubclassOf<UP13PlayerStatsWidget> PlayerStatClass;

private:
	UPROPERTY()
	UP13InventoryStatsWidget* InventoryStatsWidget = nullptr;
	UPROPERTY()
	UP13HealthWidget* HealthWidget = nullptr;
	UPROPERTY()
	UP13PlayerStatsWidget* PlayerStatWidget = nullptr;
};
