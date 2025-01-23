// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Player/P10AIGuard.h"
#include "P10GuardStateWidget.generated.h"

class AP10AIGuard;
class UTextBlock;

UCLASS()
class PROJECT10_API UP10GuardStateWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetGuard(AP10AIGuard* NewGuard);

protected:
	void OnStateChangesHandle(EP10AIGuardState GuardState);
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* StateTextBlock;

private:
	UPROPERTY()
	AP10AIGuard* Guard;
};
