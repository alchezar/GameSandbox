// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Game/P11SavePlayerInfo.h"
#include "GameFramework/Character.h"
#include "P11Character.generated.h"

class AP11HUD;
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

UENUM()
enum class EP11CharState : uint8
{
	Alive,
	Shoot,
	Dead
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FP11OnHealthChangedSignature, float, Health);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FP11OnAmmoChangedSignature, int32, Ammo);

UCLASS()
class PROJECT11_API AP11Character : public ACharacter
{
	GENERATED_BODY()

public:
	AP11Character();
	virtual void Tick(float DeltaTime) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	FORCEINLINE float GetHealth() const { return Health; }
	FORCEINLINE int32 GetAmmo() const { return Ammo; }
	void UpdateMeshOnServer(const EP11PlayerSide NewSide);

protected:
	virtual void BeginPlay() override;
	void MoveInput(const FInputActionValue& ActionValue);
	void LookInput(const FInputActionValue& ActionValue);
	void CrouchInput();

	UFUNCTION(Client, Reliable)
	void ShowInterface();
	UFUNCTION(Client, Reliable)
	void HideInterface();

	void FireInput();
	UFUNCTION(Server, Reliable)
	void ServerFire();
	void Fire();
	UFUNCTION(Client, Reliable)
	void Client_Trace();
	UFUNCTION(Server, Reliable)
	void Server_Trace();
	void ShotTrace(FHitResult& HitResult);
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Ragdoll();
	void RespawnHandle();

	void ShiftCameraInput();
	UFUNCTION(Server, Reliable)
	void Server_ShiftCamera(EP11CameraSide NewSide, float NewShift);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ShiftCamera(EP11CameraSide NewSide, float NewShift);
	void ShiftCameraSmoothly(EP11CameraSide NewSide, const float TargetShift);

	UFUNCTION(Server, Unreliable)
	void Server_Audio(USoundBase* SoundToPlay);
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_Audio(USoundBase* SoundToPlay);


private:
	void FindDefaultReferences();
	void GetSubsystem();
	void DrawDebugShoot(const FHitResult& HitResult, const float Time) const;
	AP11HUD* GetPlayerHUD();
	UFUNCTION()
	void OnRep_Health();
	UFUNCTION()
	void OnRep_Ammo();
	void ReturnShootAbility(const EP11CharState StateBeforeShoot);
	UFUNCTION()
	void OnRep_PlayerSide();
	void UpdateMaterials() const;
	void AddScore(const AController* Killer);

public:
	FP11OnHealthChangedSignature OnHealthChanged;
	FP11OnAmmoChangedSignature OnAmmoChanged;
	UPROPERTY(ReplicatedUsing = "OnRep_PlayerSide")
	EP11PlayerSide PlayerSide = EP11PlayerSide::Jedi;
	
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

	UPROPERTY(Replicated, EditDefaultsOnly, Category = "C++ | Attribute")
	EP11CharState CharState = EP11CharState::Alive;
	UPROPERTY(Replicated, EditDefaultsOnly, Category = "C++ | Attribute")
	float MaxHealth = 100.f;
	UPROPERTY(ReplicatedUsing = "OnRep_Health", EditDefaultsOnly, Category = "C++ | Attribute")
	float Health = 0.f;
	UPROPERTY(Replicated, EditDefaultsOnly, Category = "C++ | Attribute")
	int32 MaxAmmo = 30;
	UPROPERTY(ReplicatedUsing = "OnRep_Ammo", EditDefaultsOnly, Category = "C++ | Attribute")
	int32 Ammo = 0;

	UPROPERTY(EditAnywhere, Category = "C++ | Sound")
	USoundBase* FireSound;
	UPROPERTY(EditAnywhere, Category = "C++ | Sound")
	USoundBase* NoAmmoSound;

private:
	bool bCrouch = false;
	FTimerHandle DrawUIHandle;
	FTimerHandle CameraShiftTimer;
	FTimerHandle ShootHandle;
};
