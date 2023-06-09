// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ARAICharacter.generated.h"

class UPawnSensingComponent;

UCLASS()
class GAMESANDBOX_API AARAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AARAICharacter();
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

protected:
	UPROPERTY(EditAnywhere, Category = "C++ | Component")
	UPawnSensingComponent* PawnSensingComp;
};
