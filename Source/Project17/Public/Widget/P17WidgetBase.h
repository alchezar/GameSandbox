// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P17WidgetBase.generated.h"

class UP17UIEnemyComponent;
class UP17UIHeroComponent;

UCLASS()
class PROJECT17_API UP17WidgetBase : public UUserWidget
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
protected:
	virtual void NativeOnInitialized() override;

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
	UFUNCTION(BlueprintCallable)
	void InitEnemyCreatedWidget(AActor* OwningEnemyActor);

protected:
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnOwningHeroUIComponentInit"))
	void BP_OnOwningHeroUIComponentInit(UP17UIHeroComponent* OwningHeroUIComponent);
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnOwningEnemyUIComponentInit"))
	void BP_OnOwningEnemyUIComponentInit(UP17UIEnemyComponent* OwningEnemyUIComponent);
};
