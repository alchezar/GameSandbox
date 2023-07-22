// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P8MainMenuWidget.generated.h"

class UEditableTextBox;
class UP8GameInstance;
class UButton;

UCLASS()
class GAMESANDBOX_API UP8MainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;
	
protected:
	virtual void NativeConstruct() override;
	UFUNCTION()
	void OnHostClickedHandle();
	UFUNCTION()
	void OnJoinClickedHandle();
	
protected:
	UPROPERTY(meta = (BindWidget))
	UButton* HostButton;
	UPROPERTY(meta = (BindWidget))
	UButton* JoinButton;
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* AddressText;
	
private:
	UPROPERTY()
	UP8GameInstance* GameInstance;
};
