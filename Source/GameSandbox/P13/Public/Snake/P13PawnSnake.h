// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Pawn.h"
#include "P13PawnSnake.generated.h"

class UInputAction;
class UInputMappingContext;
class AP13SnakeBase;
class UCameraComponent;

UCLASS()
class GAMESANDBOX_API AP13PawnSnake : public APawn
{
	GENERATED_BODY()

	/* ------------------------------- Super ------------------------------- */
public:
	AP13PawnSnake();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	/* ------------------------------- This -------------------------------- */
protected:
	void MoveInput(const FInputActionValue& Value);

private:
	void AddDefaultMappingContext();
	void CreateSnakeActor();

	/* ------------------------------ Members ------------------------------ */
protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "C++")
	UCameraComponent* PawnCamera;
	UPROPERTY(EditAnywhere, Category = "C++")
	TSubclassOf<AP13SnakeBase> SnakeClass;

	UPROPERTY(EditAnywhere, Category = "C++ | Input")
	UInputMappingContext* DefaultContext = nullptr;
	UPROPERTY(EditAnywhere, Category = "C++ | Input")
	UInputAction* MoveAction = nullptr;

private:
	TWeakObjectPtr<AP13SnakeBase> CachedSnake;
};
