// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P11ChatMessage.generated.h"

class UTextBlock;

UENUM(BlueprintType)
enum class EP11MessageState : uint8
{
	Player UMETA(DisplayName = "From player"),
	Login  UMETA(DisplayName = "After login"),
	Logout UMETA(DisplayName = "After logout")
};

UCLASS()
class PROJECT11_API UP11ChatMessage : public UUserWidget
{
	GENERATED_BODY()

public:
	void AddMessage(const FString& Sender, const FString& Message);

protected:
	virtual void NativeConstruct() override;
	void SetTextBlocks();

public:
	EP11MessageState MessageState = EP11MessageState::Player;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FString SenderString;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FString MessageString;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SenderText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MessageText;
};
