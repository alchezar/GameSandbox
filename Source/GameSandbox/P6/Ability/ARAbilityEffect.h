// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "ARAbility.h"
#include "ARAbilityEffect.generated.h"


UCLASS()
class GAMESANDBOX_API UARAbilityEffect : public UARAbility
{
	GENERATED_BODY()

public:
	UARAbilityEffect();
	virtual void StartAbility_Implementation(AActor* Instigator) override;
	virtual void StopAbility_Implementation(AActor* Instigator) override;

	float GetTimeRemaining() const;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
protected:
	UFUNCTION(BlueprintNativeEvent, Category = "C++ | Effect")
	void ExecutePeriodicEffect(AActor* Instigator);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Effect")
	float Duration;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Effect")
	float Period;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Effect")
	float DamageAmount = 4.f;

	UPROPERTY(Replicated)
	float TimeStarted;

private:
	FTimerHandle DurationTimer;
	FTimerHandle PeriodTimer;
};
