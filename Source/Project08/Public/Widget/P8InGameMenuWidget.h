// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P8InGameMenuWidget.generated.h"

class IP8PlayerControllerInterface;
class UButton;

UCLASS()
class PROJECT08_API UP8InGameMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;
	FORCEINLINE IP8PlayerControllerInterface* GetPCInterface() const { return PCInterface; };
	void SetPCInterface(IP8PlayerControllerInterface* NewPCInterface);

protected:
	virtual void NativeConstruct() override;
	UFUNCTION()
	void OnCancelClicked();
	UFUNCTION()
	void OnQuitClicked();
	
protected:
	UPROPERTY(meta = (BindWidget))
	UButton* CancelButton;
	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;

private:
	IP8PlayerControllerInterface* PCInterface;
};
