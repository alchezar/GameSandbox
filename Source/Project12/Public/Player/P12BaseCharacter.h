// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "InputActionValue.h"
#include "Controller/P12PlayerController.h"
#include "GameFramework/Character.h"
#include "Inventory/P12InventoryItem.h"
#include "Subsystem/SaveSubsystem/P12SaveSubsystemInterface.h"
#include "Util/P12CoreTypes.h"
#include "P12BaseCharacter.generated.h"

class UP12InventoryComponent;
class AP12EquipableItem;
class UWidgetComponent;
class IP12Interactable;
class AP12HUD;
class AP12RangeWeaponItem;
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

	FP12CameraArmLength() {}

	UPROPERTY(EditAnywhere)
	float Crouch = 150.f;
	UPROPERTY(EditAnywhere)
	float Walk = 200.f;
	UPROPERTY(EditAnywhere)
	float Run = 300.f;
	UPROPERTY(EditAnywhere)
	float Aim = 100.f;
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

DECLARE_MULTICAST_DELEGATE_TwoParams(FP12OnHealthChangeSignature, float /*Health*/, float /*MaxHealth*/)
DECLARE_MULTICAST_DELEGATE_OneParam(FP12OnAimingStateChangedSignature, bool /*bStart*/)
DECLARE_MULTICAST_DELEGATE_TwoParams(FP12OnAmmoCountChangedSignature, int32 /*AmmoCount*/, int32 /*TotalAmmo*/)
DECLARE_MULTICAST_DELEGATE_OneParam(FP12OnReloadCompleteSignature, bool /*bReloaded*/)
DECLARE_MULTICAST_DELEGATE_OneParam(FP12OnLoadoutCreatedSignature, UP12EquipmentComponent* /*Equipment*/)
DECLARE_MULTICAST_DELEGATE_OneParam(FP12OnInteractableObjectFoundSignature, bool/* bFound*/)

UCLASS()
class PROJECT12_API AP12BaseCharacter : public ACharacter, public IGenericTeamAgentInterface, public IP12SaveSubsystemInterface
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
	FORCEINLINE bool GetIsAiming() const { return bAiming; }
	FORCEINLINE bool GetIsEquipping() const { return bEquipping; }
	FORCEINLINE UAnimMontage* GetAttachFromTopMontage() const { return AttachFromTopMontage; }
	FORCEINLINE UP12EquipmentComponent* GetEquipmentComponent() const { return Equipment; }
	FORCEINLINE UP12AttributeComponent* GetAttributeComponent() const { return CharacterAttribute; }
	UP12BaseCharacterMovementComponent* GetBaseCharacterMovement() const;
	FRotator GetLocalAimOffset();
	float GetHeathPercent();
	virtual void MoveInput(const FInputActionValue& Value);
	virtual void LookInput(const FInputActionValue& Value);
	void JumpInput();
	UFUNCTION(BlueprintCallable)
	void MantleInput(const bool bForce = false);
	void CrouchInput();
	void RunInput(const bool bRunRequest);
	void LadderJumpInput();
	void LadderClimbInput(const FInputActionValue& Value);
	void RegisterInteractiveActor(AP12InteractiveActor* NewInteractiveActor);
	void UnregisterInteractiveActor(AP12InteractiveActor* OldInteractiveActor);
	const AP12Ladder* GetAvailableLadder() const;
	void FireInput(const bool bStart);
	void AimInput(const bool bStart);
	void ReloadInput();
	void OnReloadCompleteHandle(bool bReloaded);
	void EquipItemInput(const bool bNext);
	void SetIsEquipping(const bool bEquip) { bEquipping = bEquip; }
	float PlayEquippingItem(UAnimMontage* EquipMontage);
	void EndEquippingItem();
	void EquipThrowableInput();
	void PrimaryMeleeInput();
	void SecondaryMeleeInput();
	virtual void PossessedBy(AController* NewController) override;
	AP12HUD* GetHUD() const;
	void Interact();
	bool PickupItem(const TWeakObjectPtr<UP12InventoryItem> Item);
	void UseInventory(APlayerController* PlayerController);
	void ConfirmWeaponSelection(AP12PlayerController* AP12PlayerController);

	/* IGenericTeamInterface */
	virtual FGenericTeamId GetGenericTeamId() const override;
	/* IP12SaveSubsystemInterface */
	virtual void OnLevelDeserialized_Implementation() override;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual bool CanJumpInternal_Implementation() const override;
	virtual void OnJumped_Implementation() override;
	virtual void OnMantleHandle(const FP12MantleSettings& Settings, const float StartTime);
	bool GetCanRun() const;
	void OnDeath();
	void TraceLineOfSight();

private:
	void ChangeCameraArmLength(const bool bStart, const float NewArmLength, const float NewFOV = 90.f);
	void SmoothlyChangeCameraArmLength(const bool bRunStart, const float TargetLength, const float TargetFOV);
	float GetIKSocketOffset(const FName& VirtualBoneName, const float TraceHalfDistance = 50.f, const float FromBoneToBottom = 10.f);
	void LegsIKFloorAlignment();
	const FP12MantleSettings& GetMantleSettings(const float LedgeHeight) const;
	void InitAnimNotify();
	void OnEnableRagdollHandle(USkeletalMeshComponent* SkeletalMeshComponent);
	void EnableRagdoll();
	float GetDefaultCameraArmLength() const;
	float GetDefaultCameraFOV() const;
	void InitHealthProgress();

	UFUNCTION(Client, Reliable)
	void Client_ShowInterface();

public:
	FP12OnHealthChangeSignature OnHealthChange;
	FP12OnAimingStateChangedSignature OnAimingStateChanged;
	FP12OnAmmoCountChangedSignature OnAmmoCountChanged;
	FP12OnReloadCompleteSignature OnReloadComplete;
	FP12OnLoadoutCreatedSignature OnLoadoutCreated;
	FP12OnInteractableObjectFoundSignature OnInteractableObjectFound;

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
	UPROPERTY(EditAnywhere, Category = "C++ | Component")
	UWidgetComponent* HealthBar;
	UPROPERTY(EditAnywhere, Category = "C++ | Component")
	UP12InventoryComponent* Inventory;

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

	UPROPERTY(EditAnywhere, Category = "C++ | Team")
	EP12Teams Team = EP12Teams::Player;
	UPROPERTY(EditAnywhere, Category = "C++ | Line sight")
	float LineOfSightDistance = 500.f;

private:
	TSoftObjectPtr<UP12BaseCharacterMovementComponent> BaseCharacterMovement;
	bool bCrouch = false;
	bool bRunning = false;
	bool bAiming = false;
	bool bEquipping = false;
	bool bFiring = false;
	bool bReloading = false;
	// bool bMantling = false;
	FTimerHandle RunTimer;
	FVector DefaultMeshLocation;
	float IKLeftLegOffset = 0.f;
	float IKRightLegOffset = 0.f;
	float IKHitOffset = 0.f;
	TInteractiveActorsArray AvailableInteractiveActors;
	FVector CurrentFallApex = FVector::ZeroVector;

	UPROPERTY()
	TScriptInterface<IP12Interactable> LineOfSightObject;
	bool bInteractableFound = false;
};
