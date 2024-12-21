// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ARDamagePopUp.generated.h"

class UTextBlock;

UCLASS()
class PROJECT06_API UARDamagePopUp : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

	void SetDamageText(float Delta);
	void SetActorToAttach(AActor* ToAttach);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	void UpdateScreenLocation() const;
	
protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* DamageText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	float DestroyDelay = 0.2f;
	UPROPERTY()
	AActor* ActorToAttach;
	
};
