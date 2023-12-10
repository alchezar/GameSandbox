// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "P12BaseCharacter.h"
#include "P12FirstPersonCharacter.generated.h"

class AP12PlayerController;

USTRUCT(BlueprintType)
struct FP12LadderCameraLimits
{
	GENERATED_BODY()

	FP12LadderCameraLimits() {}

	FP12LadderCameraLimits(const float NewMinPitch, const float NewMaxPitch, const float NewMinYaw, const float NewMaxYaw)
		: MinPitch(NewMinPitch), MaxPitch(NewMaxPitch), MinYaw(NewMinYaw), MaxYaw(NewMaxYaw) {}

	explicit FP12LadderCameraLimits(const APlayerCameraManager* Manager)
	{
		MinPitch = Manager->ViewPitchMin;
		MaxPitch = Manager->ViewPitchMax;
		MinYaw = Manager->ViewYawMin;
		MaxYaw = Manager->ViewYawMax;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = -89.f, ClampMax = 89.f))
	float MinPitch = -40.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = -89.f, ClampMax = 89.f))
	float MaxPitch = 60.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0.f, ClampMax = 359.f))
	float MinYaw = 300.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0.f, ClampMax = 359.f))
	float MaxYaw = 60.f;
};

UCLASS()
class GAMESANDBOX_API AP12FirstPersonCharacter : public AP12BaseCharacter
{
	GENERATED_BODY()

public:
	explicit AP12FirstPersonCharacter(const FObjectInitializer& ObjectInitializer);
	virtual void PossessedBy(AController* NewController) override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual FRotator GetViewRotation() const override;
	virtual void MoveInput(const FInputActionValue& Value) override;
	virtual void LookInput(const FInputActionValue& Value) override;
	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;

protected:
	virtual void BeginPlay() override;
	virtual void OnMantleHandle(const FP12MantleSettings& Settings, const float StartTime) override;
	bool IsAnyMontagePlayingOn(const USkeletalMeshComponent* MeshComponent) const;

private:
	void SetIgnoreInput(const bool bIgnore);
	void SetCameraLimits(const FP12LadderCameraLimits Limits);
	void ResetCameraLimits();
	void TurnLadderLimits(const bool bOn);

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ | Component")
	USkeletalMeshComponent* FirstPersonMesh;
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ | Component")
	UCameraComponent* FirstPersonCamera;

	UPROPERTY(EditAnywhere, Category = "C++ | Component")
	FName SocketName = FName("CamSocketNeck");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Movement | Ladder")
	FP12LadderCameraLimits LadderCameraLimits;

private:
	FTimerHandle MontageTimer;
	TWeakObjectPtr<AP12PlayerController> CachedPlayerController;
};
