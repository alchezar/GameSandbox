// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P11ChatMessage.generated.h"

class UTextBlock;

UCLASS()
class GAMESANDBOX_API UP11ChatMessage : public UUserWidget
{
	GENERATED_BODY()

public:
	void AddMessage(const FString& Sender, const FString& Message);

protected:
	virtual void NativeConstruct() override;
	void SetTextBlocks();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FString SenderString;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FString MessageString;

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SenderText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MessageText;
};
