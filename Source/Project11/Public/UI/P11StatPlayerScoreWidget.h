// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P11StatPlayerScoreWidget.generated.h"

class UBorder;
class AP11PlayerState;
class UImage;
class UTextBlock;

UCLASS()
class PROJECT11_API UP11StatPlayerScoreWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetPlayerState(AP11PlayerState* CurrentState);
	void UpdateScore() const;

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeDestruct() override;

private:
	void SetLocalStyle() const;

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerNameText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* KillsText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* DeathText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PingText;
	UPROPERTY(meta = (BindWidget))
	UImage* DeadImg;
	UPROPERTY(meta = (BindWidget))
	UBorder* RowBorder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++", meta = (Units = "s"))
	float UpdateFrequency = 0.5f;

private:
	FTimerHandle UpdateTimer;
	UPROPERTY()
	AP11PlayerState* PlayerState;
};
