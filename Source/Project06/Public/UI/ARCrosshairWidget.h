// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ARCrosshairWidget.generated.h"

class UTextBlock;
class AARCharacter;

UCLASS()
class PROJECT06_API UARCrosshairWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

	void BindHealthToAliveBody(const AARCharacter* NewBody);
	void BindDelegates();

protected:
	virtual void NativeConstruct() override;
	UFUNCTION()
	void OnHealthChangedHandle(AActor* InstigatorActor, UARAttributesComponent* OwningComp, float NewHealth, float Delta);
	UFUNCTION()
	void OnPawnChangedHandle(APawn* NewPawn);

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlockHealth;
};
