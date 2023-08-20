// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "P10AIGuard.generated.h"

class UPawnSensingComponent;

UCLASS()
class GAMESANDBOX_API AP10AIGuard : public ACharacter
{
	GENERATED_BODY()

public:
	AP10AIGuard();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnSeePawnHandle(APawn* Pawn);
	UFUNCTION()
	void OnHearNoiseHandle(APawn* NoiseInstigator, const FVector& Location, float Volume);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	UPawnSensingComponent* PawnSensing;
};
