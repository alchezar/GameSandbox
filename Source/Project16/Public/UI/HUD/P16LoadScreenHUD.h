// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "P16LoadScreenHUD.generated.h"

class UP16MVVMLoadScreen;
class UP16LoadScreenWidget;

UCLASS()
class PROJECT16_API AP16LoadScreenHUD : public AHUD
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
public:
	virtual void BeginPlay() override;

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
	UFUNCTION(BlueprintCallable)
	UP16MVVMLoadScreen* GetLoadScreenViewModel() const { return LoadScreenViewModel; };

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++")
	TSubclassOf<UP16LoadScreenWidget> LoadScreenWidgetClass = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++")
	TSubclassOf<UP16MVVMLoadScreen> LoadScreenViewModelClass = nullptr;

private:
	UPROPERTY()
	TObjectPtr<UP16LoadScreenWidget> LoadScreenWidget = nullptr;
	UPROPERTY()
	TObjectPtr<UP16MVVMLoadScreen> LoadScreenViewModel = nullptr;
};
