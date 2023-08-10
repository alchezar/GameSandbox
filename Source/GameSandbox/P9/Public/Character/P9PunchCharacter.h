// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "P9/Public/Util/P9Utils.h"
#include "P9PunchCharacter.generated.h"

class USphereComponent;
class UCameraComponent;
class USpringArmComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;


UCLASS()
class GAMESANDBOX_API AP9PunchCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AP9PunchCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:
	virtual void BeginPlay() override;
	/* Move character. */
	void Move(const FInputActionValue& Value);
	/* Rotate controller around the character. */
	void Look(const FInputActionValue& Value);
	/* Fires when the attack button is pressed. */
	void Attack();
	/* Fires when the line trace button is pressed. */
	void FireLineTrace();
	/* The edges of the animation segment, when the fists are active and can deal damage. */
	void OnPunchHandle(USkeletalMeshComponent* MeshComp, bool bStart);
	/* The point on the animation, where the fists will makes whoosh sound. */
	void OnWhooshHandle(USkeletalMeshComponent* MeshComp);
	UFUNCTION() /* Fires when FistCollision components hits an Enemy */
	void OnAttackHitHandle(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION() /* Fires when FistCollision component enters an Enemy */
	void OnAttackBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() /* Fires when FistCollision component leaves an Enemy */
	void OnAttackEndOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	void SetupComponents();
	void SetupDefaultReferences();
	void CheckHardReferences();
	void AddMappingContext();
	void CallbackDelegates();
	void CallbackAnimNotifies();
	
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
	UInputAction* AttackAction;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	UInputAction* FireAction;
#pragma endregion /* Input */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Punch")
	UDataTable* PlayerAttackDataTable;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Punch")
	USoundBase* PunchSound;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Punch")
	USoundBase* WhooshSound;
	UPROPERTY(EditAnywhere, Category = "C++ | Trace")
	EP9LineTraceType LineTraceType = EP9LineTraceType::CAMERA;
	UPROPERTY(EditAnywhere, Category = "C++ | Trace")
	float LineTraceDistance = 100.f;	

private:
	FP9MeleeCollisionProfile MeleeCollisionProfile = FP9MeleeCollisionProfile("Weapon", "NoCollision");
	EP9CharState CharState = EP9CharState::IDLE;
	UPROPERTY()
	UAudioComponent* PunchAudioComp;
	UPROPERTY()
	UAudioComponent* WhooshAudioComp;
};
