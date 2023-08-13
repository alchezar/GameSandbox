// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "P9/Public/Interface/P9Interaction.h"
#include "P9/Public/Util/P9Utils.h"
#include "P9PunchCharacter.generated.h"

class AP9HUD;
class USphereComponent;
class UCameraComponent;
class USpringArmComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;


UCLASS()
class GAMESANDBOX_API AP9PunchCharacter : public ACharacter, public IP9Interaction
{
	GENERATED_BODY()

public:
	AP9PunchCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void Interact(ACharacter* Causer) override;
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE EP9CharState GetCharState() const { return CharState; }
	FORCEINLINE EP9CharMoving GetCharMoveState() const { return CharMoveState; }
	FORCEINLINE bool GetIsCurrentlyPossesses() const { return bCurrentlyPosses; }
	FORCEINLINE AController* GetSavedController() const { return SavedController; }
	void SetPossessedColor();

protected:
	virtual void BeginPlay() override;
	void MoveInput(const FInputActionValue& Value);
	void LookInput(const FInputActionValue& Value);
	void CrouchInput(const bool bEnable);
	void RunInput(const bool bEnable);
	void ArmInput();
	void AttackInput();
	void InteractInput();
	void OnPunchHandle(USkeletalMeshComponent* MeshComp, bool bStart);
	void OnWhooshHandle(USkeletalMeshComponent* MeshComp);
	UFUNCTION()
	void OnAttackHitHandle(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
	void OnAttackBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnAttackEndOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	void SetupComponents();
	void SetupDefaultReferences();
	void CheckHardReferences();
	void AddMappingContext();
	void CallbackAnimNotifies();
	void CallbackDelegates();
	void ResetComboHandle();
	void ArmPlayer(const bool bEnable);
	void ChangeMovingSpeed() const;

	/**
	 * Log - prints a message to all the log outputs with a specific color
	 * @param Level affects color of log
	 * @param Message the message for display
	 */
	void Log(EP9LogLevel Level, const FString& Message);
	/**
	 * Log - prints a message to all the log outputs with a specific color
	 * @param Level affects color of log
	 * @param Message the message for display
	 * @param LogOutput - All, Output Log or Screen
	 */
	void Log(const EP9LogLevel Level,  const FString& Message, const EP9LogOutput LogOutput);
	/* Draw traced line for debug purposes. */
	void TraceDebugLine(const FHitResult& Hit, const FVector& Start, const FVector& End) const;

protected:
#pragma region Component
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	USpringArmComponent* CameraBoom;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	UCameraComponent* FollowCamera;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	USphereComponent* LeftFistCollision;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	USphereComponent* RightFistCollision;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	USphereComponent* LeftLegCollision;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	USphereComponent* RightLegCollision;	
#pragma endregion /* Component */
#pragma region Input
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	UInputMappingContext* DefaultContext;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	UInputAction* MoveAction;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	UInputAction* LookAction;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	UInputAction* JumpAction;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	UInputAction* CrouchAction;	
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	UInputAction* RunAction;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	UInputAction* AttackAction;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	UInputAction* InteractAction;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	UInputAction* ArmAction;		
#pragma endregion /* Input */

	UPROPERTY(EditAnywhere, Category = "C++ | Moving")
	float MaxWalkSpeed = 600.f;
	UPROPERTY(EditAnywhere, Category = "C++ | Moving")
	float MaxCrouchSpeed = 300.f;
	UPROPERTY(EditAnywhere, Category = "C++ | Moving")
	float MaxRunSpeed = 900.f;
	UPROPERTY(EditAnywhere, Category = "C++ | Moving")
	float MaxArmedSpeed = 450.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Punch")
	UDataTable* PlayerAttackDataTable;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Punch")
	USoundBase* PunchSound;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Punch")
	USoundBase* WhooshSound;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Punch")
	float ArmedToIdleDelay = 5.f; // CountdownToIdle
	
	UPROPERTY(EditAnywhere, Category = "C++ | Interact")
	EP9LineTraceType LineTraceType = EP9LineTraceType::CAMERA;
	UPROPERTY(EditAnywhere, Category = "C++ | Interact")
	float InteractDistance = 200.f;
	UPROPERTY(EditAnywhere, Category = "C++ | Interact")
	TArray<FName> PossessedColorParameterNames = {"PaintColor", "MainColor"};

	UPROPERTY(EditAnywhere, Category = "C++ | Possession")
	FP9PlayerColor PlayerColor;

private:
	int32 CurrentComboCount = 0;
	FTimerHandle ComboTimer;
	FTimerHandle ArmTimer;
	FP9MeleeCollisionProfile MeleeCollisionProfile = FP9MeleeCollisionProfile("Weapon", "NoCollision");
	EP9CharState CharState = EP9CharState::IDLE;
	EP9CharMoving CharMoveState = EP9CharMoving::WALK;
	bool bCurrentlyPosses = false;
	UPROPERTY()
	UAudioComponent* PunchAudioComp;
	UPROPERTY()
	UAudioComponent* WhooshAudioComp;
	UPROPERTY()
	AP9HUD* HUD;
	UPROPERTY()
	AController* SavedController;
};
