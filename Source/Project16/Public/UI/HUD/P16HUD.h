// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Util/P16Type.h"
#include "P16HUD.generated.h"

class UP16AttributeMenuWidgetController;
class UP16OverlayWidgetController;
class UP16Widget;

UCLASS()
class PROJECT16_API AP16HUD : public AHUD
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
public:
	AP16HUD();

protected:
	virtual void BeginPlay() override;

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
	UP16OverlayWidgetController*       GetOverlayWidgetController(const FP16WidgetControllerParams& InParams = {});
	UP16AttributeMenuWidgetController* GetAttributeMenuWidgetController(const FP16WidgetControllerParams& InParams = {});

	void InitOverlay(const FP16WidgetControllerParams& InParams);

private:
	template <typename T>
	T* GetWidgetController(TObjectPtr<T>& InWidgetController, TSubclassOf<T> InWidgetControllerClass, const FP16WidgetControllerParams& InParams = {});

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TSubclassOf<UP16Widget> OverlayWidgetClass = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TSubclassOf<UP16OverlayWidgetController> OverlayWidgetControllerClass = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TSubclassOf<UP16AttributeMenuWidgetController> AttributeMenuWidgetControllerClass = nullptr;

private:
	UPROPERTY()
	TObjectPtr<UP16Widget> OverlayWidget = nullptr;
	UPROPERTY()
	TObjectPtr<UP16OverlayWidgetController> OverlayWidgetController = nullptr;
	UPROPERTY()
	TObjectPtr<UP16AttributeMenuWidgetController> AttributeMenuWidgetController = nullptr;
};
