// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "P13/Public/Intearface/P13InputInterface.h"
#include "P13/Public/Library/P13Types.h"
#include "P13TopDownCharacter.generated.h"

class AP13Weapon;
class USpringArmComponent;
class UCameraComponent;

USTRUCT(BlueprintType)
struct FP13CameraHeightClamp
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Min = 500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Max = 1500.f;
};

UCLASS(meta = (PrioritizeCategories = "C++"))
class GAMESANDBOX_API AP13TopDownCharacter : public ACharacter, public IP13InputInterface
{
	GENERATED_BODY()

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                                Super                                  *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
public:
	AP13TopDownCharacter();
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PossessedBy(AController* NewController) override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void Tick(const float DeltaTime) override;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                               Interface                               *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
public:
	virtual void MoveInput(const FVector2D MoveVector) override;
	virtual void SprintInput(const bool bStart) override;
	virtual void AimInput() override;
	virtual void ZoomInput(const float Axis) override;
	virtual void RotateTowardMovement(const FVector& Direction) override;
	virtual void FireInput(const bool bStart) override;
	virtual void ReloadInput() override;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                                 This                                  *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
public:
	FORCEINLINE float GetIKLeftLegOffset() const { return IKLeftLegOffset; }
	FORCEINLINE float GetIKRightLegOffset() const { return IKRightLegOffset; }
	FORCEINLINE float GetIKHipOffset() const { return IKHitOffset; }
	FORCEINLINE EP13MovementState GetMovementState() const { return MovementState; }
	FORCEINLINE AP13Weapon* GetCachedWeapon() const { return CachedWeapon.Get(); }
	void UpdateCharacter();
	void ChangeMovementState(const EP13MovementState NewMovementState);
	FVector GetLookAtCursorDirection() const;

protected:
	void OnHitUnderCursorChangedHandle(APlayerController* PlayerController, const FHitResult& HitResult);

private:
	void CreateComponents();
	void SetupCameraBoom() const;
	void ShakeCamera() const;
	void UpdateMeshRotation();
	/* Leg alignment. */
	float GetIKSocketOffset(const FName& VirtualBoneName, const float TraceHalfDistance = 50.f, const float FromBoneToBottom = 10.f);
	void LegsIKFloorAlignment();
	void ZoomSmoothly(const float DeltaTime, const float FinalLength);
	void InitWeapon(const FName WeaponID);
	void ZoomToCursor(const bool bOn);
	void ZoomToCursorSmoothly() const;
	bool CheckCharacterCanFire() const;
	void OnWeaponFiredHandle(UAnimMontage* CharFireAnim);
	void OnWeaponReloadHandle(const bool bStart, UAnimMontage* CharReloadAnim);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                               Variables                               *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
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
	FRotator CameraRotation = FRotator(-80.f, 0.f, 0.f);

	UPROPERTY(EditAnywhere, Category = "C++ | Effect")
	TSubclassOf<UCameraShakeBase> ShakeClass = nullptr;

	UPROPERTY(EditAnywhere, Category = "C++ | Movement")
	EP13MovementState MovementState = EP13MovementState::Run;
	UPROPERTY(EditAnywhere, Category = "C++ | Movement")
	FP13MovementSpeed MovementSpeed;
	UPROPERTY(EditAnywhere, Category = "C++ | Movement")
	float RotationRate = 10.f;

	UPROPERTY(EditAnywhere, Category = "C++ | Weapon")
	FName CurrentWeaponID = "Pistol";

private:
	float IKLeftLegOffset = 0.f;
	float IKRightLegOffset = 0.f;
	float IKHitOffset = 0.f;
	float ZoomSteps = 0.f;
	FTimerHandle ScrollTimer;
	FTimerHandle ZoomTimer;
	FTimerHandle AimTimer;
	EP13MovementState PreviousMovementState = MovementState;
	TWeakObjectPtr<AP13Weapon> CachedWeapon;
	FHitResult HitUnderCursor;
};