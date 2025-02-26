// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P16Widget.generated.h"

UCLASS()
class PROJECT16_API UP16Widget : public UUserWidget
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	/* ------------------------------- This -------------------------------- */
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);
	UFUNCTION(BlueprintImplementableEvent)
	void OnWidgetControllerSet();

	/* ------------------------------ Fields ------------------------------- */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++")
	TObjectPtr<UObject> WidgetController = nullptr;
};
