// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P8ServerRow.generated.h"

struct FP8ServerData;
class UButton;
class UP8MainMenuWidget;
class UTextBlock;

UCLASS()
class GAMESANDBOX_API UP8ServerRow : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetupRow(UP8MainMenuWidget* Parent, const uint32 NewIndex, const FP8ServerData& Data);
	void Unselect();

protected:
	virtual void NativeConstruct() override;
	UFUNCTION()
	void OnRowButtonClickedHandle();
	UFUNCTION()
	void OnRowButtonHoveredHandle();
	UFUNCTION()
	void OnRowButtonUnhoveredHandle();

private:
	void SetRowTextColor(const FColor NewColor);

public:
	bool bSelected = false;

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* RowButton;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ServerName;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ServerOwnerName;	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Players;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++", meta = (BindWidget))
	bool bHeader = false;
	
private:
	uint32 Index = 0;
	UPROPERTY()
	UP8MainMenuWidget* ParentWidget;
	TArray<UTextBlock*> AllText;
};
