// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Character/P13CharacterBase.h"
#include "Intearface/P13InputInterface.h"
#include "P13CharacterTopDown.generated.h"

class AP13PlayerController;

UCLASS()
class PROJECT13_API AP13CharacterTopDown : public AP13CharacterBase, public IP13InputInterface
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
	virtual void OnWeaponReloadStartHandle(UAnimMontage* CharReloadAnim, const int32 WeaponIndex, const float ReloadingTime) override;
	virtual void OnDeathHandle(AController* Causer) override;
	virtual void TryLoadSavedColor(AController* NewController) override;

	/* ------------------------------- This -------------------------------- */
protected:
	void OnHitUnderCursorChangedHandle(APlayerController* PlayerController, const FHitResult& HitResult);
	void ShowInterface();

private:
	void CreateTopDownComponents();
	void SetupCameraBoom() const;
	void ShakeCamera() const;
	void UpdateMeshRotation();
	void ZoomSmoothly(const float DeltaTime, const float FinalLength);

	void FocusOnCursor(const bool bOn);
	void FocusOnCursorSmoothly() const;
	void ToggleAim();
	void ListenToControllerCursor(AController* NewController = nullptr);

	/* ------------------------------ Network ------------------------------ */
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UFUNCTION(Server, Reliable)
	void Server_TakeNextWeapon(const bool bNext) const;
	UFUNCTION(Server, Unreliable)
	void Server_RotateTowardMovement(const FVector& Direction);
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_RotateTowardMovement(const FVector& Direction);
	UFUNCTION(Client, Reliable)
	void Client_ListenToControllerCursor(AController* NewController);
	UFUNCTION(Server, Reliable)
	void Server_PullTrigger(const bool bStart) const;
	UFUNCTION(Server, Unreliable)
	void Server_UpdateTargetLocation(const FVector& TargetLocation);
	UFUNCTION(Server, Reliable)
	void Server_TryReloadWeapon();
	UFUNCTION(Server, Reliable)
	void Server_ToggleAim();
	UFUNCTION(Client, Reliable)
	void Client_FocusOnCursor(const bool bOn);

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
	bool bTriggerPulled = false;
	FTimerHandle ScrollTimer;
	FTimerHandle ZoomTimer;
	FTimerHandle AimTimer;
	FHitResult HitUnderCursor;
};
