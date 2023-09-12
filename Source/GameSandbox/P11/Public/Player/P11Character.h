// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "P11Character.generated.h"

struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
class UCameraComponent;
class USpringArmComponent;

UENUM()
enum class EP11CameraSide : uint8
{
	Left,
	Right
};

UCLASS()
class GAMESANDBOX_API AP11Character : public ACharacter
{
	GENERATED_BODY()

public:
	AP11Character();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
	void MoveInput(const FInputActionValue& ActionValue);
	void LookInput(const FInputActionValue& ActionValue);
	void CrouchInput();

	void FireInput();
	UFUNCTION(Server, Reliable)
	void ServerFire();
	void Fire();

	void ShiftCameraInput();
	UFUNCTION(Server, Reliable)
	void Server_ShiftCamera(EP11CameraSide NewSide, float NewShift);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ShiftCamera(EP11CameraSide NewSide, float NewShift);
	void ShiftCameraSmoothly(EP11CameraSide NewSide, const float TargetShift);

private:
	void FindDefaultReferences();
	void GetSubsystem();
	void DrawDebugShoot(const FHitResult& HitResult, const float Time) const;

protected:
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	UCameraComponent* FollowCamera;

#pragma region Input
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	UInputMappingContext* DefaultContext;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	UInputAction* MoveAction;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	UInputAction* LookAction;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	UInputAction* CrouchAction;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	UInputAction* JumpAction;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	UInputAction* FireAction;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	UInputAction* AimAction;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	UInputAction* ReloadAction;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	UInputAction* ShiftCameraAction;
#pragma endregion /* Input */
	
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Camera")
	EP11CameraSide CameraSide = EP11CameraSide::Right;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Camera")
	float CameraOffset = 75.f;
	
private:
	bool bCrouch = false;
	FTimerHandle CameraShiftTimer;
};
