// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P12NetworkWidget.generated.h"

DECLARE_MULTICAST_DELEGATE(FP12OnNetworkWidgetCloseSignature)

UCLASS()
class GAMESANDBOX_API UP12NetworkWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	FText GetNetworkType() const;
	virtual void ToggleNetworkType();
	virtual void CloseWidget();

public:
	FP12OnNetworkWidgetCloseSignature OnNetworkWidgetClose;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++")
	bool bLAN = false;
};
