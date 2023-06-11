// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ARCrosshairWidget.generated.h"

class UTextBlock;
class AARCharacter;

UCLASS()
class GAMESANDBOX_API UARCrosshairWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

	void SetHealthText(const float NewHealth);
	void BindHealthToAliveBody(const AARCharacter* NewBody);
	// void RebindDelegates();

protected:
	virtual void NativeConstruct() override;
	UFUNCTION()
	void OnHealthChangedHandle(AActor* InstigatorActor, UARAttributesComponent* OwningComp, float NewHealth, float Delta);

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlockHealth;

private:
	UPROPERTY()
	AARCharacter* LastPlayer;

};
