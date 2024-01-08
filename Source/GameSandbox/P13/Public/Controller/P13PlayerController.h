// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "P13PlayerController.generated.h"

class UNiagaraSystem;
class UInputAction;
class UInputMappingContext;

DECLARE_MULTICAST_DELEGATE_TwoParams(FP13OnHitUnderCursorChangedSignature, APlayerController*, const FHitResult&)

UCLASS()
class GAMESANDBOX_API AP13PlayerController : public APlayerController
{
	GENERATED_BODY()

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                                Super                                  *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
public:
	AP13PlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void SetPawn(APawn* InPawn) override;
	virtual void OnUnPossess() override;

public:
	virtual void Tick(const float DeltaSeconds) override;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                                 This                                  *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
protected:
	void OnInputStarted();
	/* Triggered every frame when the input is held down. */
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();

	void MoveInput(const FInputActionValue& Value);
	void LookInput(const FInputActionValue& Value);
	void SprintInput(const bool bStart);
	void AimInput();
	void ZoomInput(const FInputActionValue& Value);
	void FireInput(bool const bStart);
	void ReloadInput();
	void SwitchWeaponInput(const bool bNext);
	void DropInput();

private:
	void AddDefaultMappingContext();
	void RotateTowardCursorSmoothly(const FVector Direction);
	void FindPointUnderCursor();
	void SpawnCursorDecal();
	void UpdateCursorDecalPosition() const;
	bool GetCanControlledCharacterMove() const;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                               Variables                               *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
public:
	FP13OnHitUnderCursorChangedSignature OnHitUnderCursorChanged;

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
	UInputAction* FireAction = nullptr;
	UPROPERTY(EditAnywhere, Category = "C++ | Input")
	UInputAction* ReloadAction = nullptr;
	UPROPERTY(EditAnywhere, Category = "C++ | Input")
	UInputAction* NextWeaponAction = nullptr;
	UPROPERTY(EditAnywhere, Category = "C++ | Input")
	UInputAction* PreviousWeaponAction = nullptr;
	UPROPERTY(EditAnywhere, Category = "C++ | Input")
	UInputAction* DropAction = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "C++ | Cursor")
	float ShortPressThreshold = 0.15f;
	UPROPERTY(EditAnywhere, Category = "C++ | Cursor")
	UNiagaraSystem* CursorClickFX = nullptr;
	UPROPERTY(EditAnywhere, Category = "C++ | Cursor")
	UMaterialInterface* CursorMaterial = nullptr;
	UPROPERTY(EditAnywhere, Category = "C++ | Cursor")
	FVector CursorSize = FVector(20.f, 40.f, 40.f);

private:
	FVector CachedDestination;
	float FollowTime = 0.f; /* For how long it has been pressed. */
	FHitResult HitUnderCursor;
	TWeakObjectPtr<UDecalComponent> CachedCursorDecal;
};
