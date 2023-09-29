// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "P11ServerSingle.h"
#include "P11ServerCreate.generated.h"

class UCheckBox;
class UEditableTextBox;

DECLARE_MULTICAST_DELEGATE(FP11OnServerCreateBackButtonPressedSignature)

UCLASS()
class GAMESANDBOX_API UP11ServerCreate : public UP11ServerSingle
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void OnBackHandle() override;
	virtual void OnCreateHandle() override;

public:
	FP11OnServerCreateBackButtonPressedSignature OnCreateBackButtonPressed;

protected:
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* ServerNameTextBox;
	UPROPERTY(meta = (BindWidget))
	UCheckBox* LanCheckBox;
};
