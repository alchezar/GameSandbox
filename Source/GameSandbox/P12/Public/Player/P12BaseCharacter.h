// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "P12BaseCharacter.generated.h"

class UP12WeaponBarrelComponent;
class UP12EquipmentComponent;
class UP12AttributeComponent;
class AP12Ladder;
class AP12InteractiveActor;
class UP12LedgeDetectionComponent;
class UP12BaseCharacterMovementComponent;
class UCameraComponent;
class USpringArmComponent;
class UInputAction;
class UInputMappingContext;

USTRUCT(BlueprintType)
struct FP12CameraArmLength
{
	GENERATED_BODY()

	FP12CameraArmLength() : Crouch(150.f), Walk(200.f), Run(300.f) {}

	UPROPERTY(EditAnywhere)
	float Crouch = 0.f;
	UPROPERTY(EditAnywhere)
	float Walk = 0.f;
	UPROPERTY(EditAnywhere)
	float Run = 0.f;
};

USTRUCT(BlueprintType)
struct FP12MantleSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UAnimMontage* Montage;
	UPROPERTY(EditAnywhere)
	UCurveVector* Curve;
	UPROPERTY(EditAnywhere, meta = (ClampMin = 0.f, UIMin = 0.f))
	float MinHeight = 100.f;
	UPROPERTY(EditAnywhere, meta = (ClampMin = 0.f, UIMin = 0.f))
	float MaxHeight = 200.f;
	UPROPERTY(EditAnywhere, meta = (ClampMin = 0.f, UIMin = 0.f))
	float MinHeightStartTime = 0.5f;
	UPROPERTY(EditAnywhere, meta = (ClampMin = 0.f, UIMin = 0.f))
	float MaxHeightStartTime = 0.f;
	UPROPERTY(EditAnywhere, meta = (ClampMin = 0.f, UIMin = 0.f))
	float AnimationCorrectionXY = 65.f;
	UPROPERTY(EditAnywhere, meta = (ClampMin = 0.f, UIMin = 0.f))
	float AnimationCorrectionZ = 200.f;
};

typedef TArray<AP12InteractiveActor*, TInlineAllocator<8>> TInteractiveActorsArray;

UCLASS()
class GAMESANDBOX_API AP12BaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	explicit AP12BaseCharacter(const FObjectInitializer& ObjectInitializer);
	virtual void Tick(const float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	virtual void Falling() override;
	virtual void NotifyJumpApex() override;
	virtual void Landed(const FHitResult& Hit) override;
	FORCEINLINE float GetIKLeftLegOffset() const { return IKLeftLegOffset; }
	FORCEINLINE float GetIKRightLegOffset() const { return IKRightLegOffset; }
	FORCEINLINE float GetIKHipOffset() const { return IKHitOffset; }
	FORCEINLINE UAnimMontage* GetAttachFromTopMontage() const { return AttachFromTopMontage; }
	FORCEINLINE UP12EquipmentComponent* GetEquipmentComponent() const { return Equipment; }
	UP12BaseCharacterMovementComponent* GetBaseCharacterMovement() const;
	virtual void MoveInput(const FInputActionValue& Value);
	virtual void LookInput(const FInputActionValue& Value);
	void JumpInput();
	void MantleInput(const bool bForce = false);
	void CrouchInput();
	void RunInput(const bool bRunRequest);
	void LadderJumpInput();
	void LadderClimbInput(const FInputActionValue& Value);
	void RegisterInteractiveActor(AP12InteractiveActor* NewInteractiveActor);
	void UnregisterInteractiveActor(AP12InteractiveActor* OldInteractiveActor);
	const AP12Ladder* GetAvailableLadder() const;
	void FireInput();

protected:
	virtual void BeginPlay() override;
	virtual bool CanJumpInternal_Implementation() const override;
	virtual void OnJumped_Implementation() override;
	virtual void OnMantleHandle(const FP12MantleSettings& Settings, const float StartTime);
	bool GetCanRun() const;
	void OnDeath();
	
private:
	void ChangeCameraArmLength(const bool bStart, const float NewArmLength);
	void SmoothlyChangeCameraArmLength(const bool bRunStart, const float TargetLength);
	float GetIKSocketOffset(const FName& VirtualBoneName, const float TraceHalfDistance = 50.f, const float FromBoneToBottom = 10.f);
	void LegsIKFloorAlignment();
	const FP12MantleSettings& GetMantleSettings(const float LedgeHeight) const;
	void InitAnimNotify();
	void OnEnableRagdollHandle(USkeletalMeshComponent* SkeletalMeshComponent);
	void EnableRagdoll();

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ | Component")
	USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ | Component")
	UCameraComponent* Camera;
	UPROPERTY(EditAnywhere, Category = "C++ | Component")
	UP12LedgeDetectionComponent* LedgeDetection;
	UPROPERTY(EditAnywhere, Category = "C++ | Component")
	UP12AttributeComponent* CharacterAttribute;
	UPROPERTY(EditAnywhere, Category = "C++ | Component")
	UP12EquipmentComponent* Equipment;
	
	UPROPERTY(EditAnywhere, Category = "C++ | Camera")
	FP12CameraArmLength CameraArmLength;
	
	UPROPERTY(EditAnywhere, Category = "C++ | Mantle")
	FP12MantleSettings HighMantleSettings;
	UPROPERTY(EditAnywhere, Category = "C++ | Mantle")
	FP12MantleSettings LowMantleSettings;
	UPROPERTY(EditAnywhere, Category = "C++ | Mantle")
	float LowMantleMaxHeight = 100.f;

	UPROPERTY(EditAnywhere, Category = "C++ | Montage")
	UAnimMontage* AttachFromTopMontage;
	UPROPERTY(EditAnywhere, Category = "C++ | Montage")
	UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, Category = "C++ | Damage")
	UCurveFloat* FallDamageCurve;

	
private:
	TSoftObjectPtr<UP12BaseCharacterMovementComponent> BaseCharacterMovement;
	float DefaultMaxSpeed = 0.f;
	bool bCrouch = false;
	bool bRunning = false;
	FTimerHandle RunTimer;
	FVector DefaultMeshLocation;
	float IKLeftLegOffset = 0.f;
	float IKRightLegOffset = 0.f;
	float IKHitOffset = 0.f;
	TInteractiveActorsArray AvailableInteractiveActors;
	FVector CurrentFallApex = FVector::ZeroVector;
};
