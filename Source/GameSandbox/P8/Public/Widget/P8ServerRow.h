// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P8ServerRow.generated.h"

class UButton;
class UP8MainMenuWidget;
class UTextBlock;

UCLASS()
class GAMESANDBOX_API UP8ServerRow : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetupRow(UP8MainMenuWidget* Parent, const uint32 NewIndex, const FText& NewName);

protected:
	virtual void NativeConstruct() override;
	UFUNCTION()
	void OnRowButtonClickedHandle();

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ServerName;
	UPROPERTY(meta = (BindWidget))
	UButton* RowButton;

private:
	uint32 Index = 0;
	UPROPERTY()
	UP8MainMenuWidget* ParentWidget;
};
