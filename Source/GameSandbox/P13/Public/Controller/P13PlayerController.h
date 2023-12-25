// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "P13PlayerController.generated.h"

class UNiagaraSystem;
class UInputAction;
class UInputMappingContext;

UCLASS()
class GAMESANDBOX_API AP13PlayerController : public APlayerController
{
	GENERATED_BODY()

	/* ------------------------------- Super ------------------------------- */
public:
	AP13PlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	/* ------------------------------- This -------------------------------- */
protected:
	void OnInputStarted();
	/* Triggered every frame when the input is held down. */
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();

	void MoveInput(const FInputActionValue& Value);
	void LookInput(const FInputActionValue& Value);
	void SprintInput(const bool bStart);
	void AimInput(const bool bStart);
	void ZoomInput(const FInputActionValue& Value);

private:
	void AddDefaultMappingContext();
	void RotateTowardCursorSmoothly(const FVector Direction);

	/* ----------------------------- Variables ----------------------------- */
protected:
	UPROPERTY(EditAnywhere, Category = "C++ | Input")
	UInputMappingContext* DefaultContext = nullptr;
	UPROPERTY(EditAnywhere, Category = "C++ | Input")
	UInputAction* MoveAction = nullptr;
	UPROPERTY(EditAnywhere, Category = "C++ | Input")
	UInputAction* LookAction = nullptr;
	UPROPERTY(EditAnywhere, Category = "C++ | Input")
	UInputAction* SetDestinationAction = nullptr;
	UPROPERTY(EditAnywhere, Category = "C++ | Input")
	UInputAction* SprintAction = nullptr;
	UPROPERTY(EditAnywhere, Category = "C++ | Input")
	UInputAction* AimAction = nullptr;
	UPROPERTY(EditAnywhere, Category = "C++ | Input")
	UInputAction* ZoomAction = nullptr;

	UPROPERTY(EditAnywhere, Category = "C++ | Input")
	float ShortPressThreshold = 0.15f;
	UPROPERTY(EditAnywhere, Category = "C++ | Input")
	UNiagaraSystem* FXCursor = nullptr;

private:
	FVector CachedDestination;
	/* For how long it has been pressed. */
	float FollowTime = 0.f;
	FTimerHandle RotationTickTimer;
	FTimerHandle RotationTimer;
};
