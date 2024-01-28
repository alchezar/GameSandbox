// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "P13/Public/Character/P13CharacterBase.h"
#include "P13/Public/Intearface/P13InputInterface.h"
#include "P13CharacterTopDown.generated.h"

UCLASS()
class GAMESANDBOX_API AP13CharacterTopDown : public AP13CharacterBase, public IP13InputInterface
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
public:
	AP13CharacterTopDown();
	virtual void PostInitializeComponents() override;
	virtual void PossessedBy(AController* NewController) override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void Tick(const float DeltaTime) override;

	/* ----------------------------- Interface ----------------------------- */
public:
	virtual void MoveInput(const FVector2D MoveVector) override;
	virtual void SprintInput(const bool bStart) override;
	virtual void AimInput() override;
	virtual void ZoomInput(const float Axis) override;
	virtual void RotateTowardMovement(const FVector& Direction) override;
	virtual void FireInput(const bool bStart) override;
	virtual void ReloadInput() override;
	virtual void SwitchWeaponInput(const bool bNext) override;
	virtual void DropInput(const bool bTakeNext) override;

	/* ------------------------------- Super ------------------------------- */
public:
	virtual FVector GetLookAtCursorDirection() const override;

protected:
	virtual void OnDeathHandle(AController* Causer) override;
	
	/* ------------------------------- This -------------------------------- */
protected:
	void OnHitUnderCursorChangedHandle(APlayerController* PlayerController, const FHitResult& HitResult);

private:
	void CreateTopDownComponents();
	void SetupCameraBoom() const;
	void ShakeCamera() const;
	void UpdateMeshRotation();
	void ZoomSmoothly(const float DeltaTime, const float FinalLength);

	void FocusOnCursor(const bool bOn);
	void FocusOnCursorSmoothly() const;

	/* ----------------------------- Variables ----------------------------- */
protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ | Component")
	UCameraComponent* TopDownCamera;
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ | Component")
	USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere, Category = "C++ | Camera")
	bool bFlow = true;
	UPROPERTY(EditAnywhere, Category = "C++ | Camera")
	float Height = 1000.f;
	UPROPERTY(EditAnywhere, Category = "C++ | Camera")
	FP13CameraHeightClamp HeightClamp;
	UPROPERTY(EditAnywhere, Category = "C++ | Camera")
	float ZoomStepDistance = 100.f;
	UPROPERTY(EditAnywhere, Category = "C++ | Camera")
	float SmoothCoefficient = 5.f;
	UPROPERTY(EditAnywhere, Category = "C++ | Camera")
	FRotator CameraRotation = FRotator(0.f, -70.f, 0.f);

	UPROPERTY(EditAnywhere, Category = "C++ | Effect")
	TSubclassOf<UCameraShakeBase> ShakeClass = nullptr;

private:
	float ZoomSteps = 0.f;
	FTimerHandle ScrollTimer;
	FTimerHandle ZoomTimer;
	FTimerHandle AimTimer;
	FHitResult HitUnderCursor;
};
