// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "CharacterTypes.h"
#include "P7/Public/Player/P7BaseCharacter.h"
#include "P7Character.generated.h"

struct FInputActionValue;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class AP7Item;

UCLASS()
class GAMESANDBOX_API AP7Character : public AP7BaseCharacter
{
	GENERATED_BODY()

public:
	AP7Character();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetHit(const FVector& HitterLocation) override;
	virtual bool GetIsAttaching() override;
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; };
	FORCEINLINE EActionState GetActionState() const { return ActionState; };
	void SetOverlappingItem(AP7Item* Item);

protected:
	virtual void BeginPlay() override;
	virtual bool CanAttack() override;
	virtual void Attack() override;
	virtual void OnAttackEndHandle(USkeletalMeshComponent* MeshComp) override;
	virtual void OnBeamTurningHandle(USkeletalMeshComponent* MeshComp) override;
	virtual void OnBeltSnappingHandle(USkeletalMeshComponent* MeshComp) override;
	virtual void OnHitReactEndHandle(USkeletalMeshComponent* MeshComp) override;
	void Grab();

private:
	void EquipWeapon(AP7Weapon* Weapon);
	void SetupComponents();
	void OrientToMovement(const bool bOrient) const;
	void AddMappingContext() const;
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Block(bool bBlock);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	USpringArmComponent* CameraBoom;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	UCameraComponent* ViewCamera;

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
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	UInputAction* BlockAction;
#pragma endregion // Input

private:
	UPROPERTY()
	UEnhancedInputComponent* EnhancedInputComponent;
	UPROPERTY(VisibleInstanceOnly)
	AP7Item* OverlappingItem;
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;
	EActionState ActionState = EActionState::EAS_Unoccupied;
};
