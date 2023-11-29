// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P12AttributeProgressBarWidget.generated.h"

class AP12BaseCharacter;
class UProgressBar;

UCLASS()
class GAMESANDBOX_API UP12AttributeProgressBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	void SetProgressPercentage(float Percentage);
	void OnHealthChangedHandle(float Health, float MaxHealth);
	void CacheOwner(AP12BaseCharacter* BaseChar);
	
protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ProgressBar;
	
private:
	TWeakObjectPtr<AP12BaseCharacter> CachedCharacter;	
};
