// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "CharacterTypes.h"
#include "GameFramework/Character.h"
#include "P7Character.generated.h"

struct FInputActionValue;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class AP7Item;
class AP7Weapon;
class UAnimMontage;

UCLASS()
class GAMESANDBOX_API AP7Character : public ACharacter
{
	GENERATED_BODY()

public:
	AP7Character();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Jump() override;
	virtual void Landed(const FHitResult& Hit) override;
	FORCEINLINE bool  GetIsDoubleJump() const { return bDoubleJump; };
	FORCEINLINE FName GetSocketName()   const { return SocketName; };
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; };
	void SetOverlappingItem(AP7Item* Item);

protected:
	virtual void BeginPlay() override;

private:
	void SetupComponents();
	void OrientToMovement(const bool bOrient) const;
	void AddMappingContext() const;
	void InitAnimNotifies();
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Grab();
	void Attack();
	void PlayAttackMontage();
	void OnAttackEndHandle(USkeletalMeshComponent* MeshComp);
	void OnBeamTurningHandle(USkeletalMeshComponent* MeshComp);
	void OnHiltVisibilityHandle(USkeletalMeshComponent* MeshComp);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	USpringArmComponent* CameraBoom;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	UCameraComponent* ViewCamera;

	UPROPERTY(EditDefaultsOnly, Category = "C++ | Weapon")
	FName SocketName = "GripPoint";

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
	UInputAction* GrabAction;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	UInputAction* AttackAction;
#pragma endregion // Input

#pragma region Montage
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Montage")
	UAnimMontage* AttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Montage")
	UAnimMontage* EquipMontage;
#pragma endregion // Montage

private:
	UPROPERTY()
	UEnhancedInputComponent* EnhancedInputComponent;
	UPROPERTY(VisibleInstanceOnly)
	AP7Item* OverlappingItem;
	UPROPERTY(VisibleInstanceOnly)
	AP7Weapon* EquippedWeapon;
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;
	EActionState ActionState = EActionState::EAS_Unoccupied;
	bool bDoubleJump = false;
	int32 Section = 0;
	FTimerHandle ComboTimer;
};
