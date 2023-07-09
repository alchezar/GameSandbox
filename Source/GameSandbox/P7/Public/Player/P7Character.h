// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "CharacterTypes.h"
#include "P7/Public/Interface/P7PickupInterface.h"
#include "P7/Public/Player/P7BaseCharacter.h"
#include "P7Character.generated.h"

struct FInputActionValue;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class AP7Item;
class UP7PlayerOverlay;

UCLASS()
class GAMESANDBOX_API AP7Character : public AP7BaseCharacter, public IP7PickupInterface
{
	GENERATED_BODY()

public:
	AP7Character();
	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetHit(const FVector& HitterLocation) override; // AP7BaseCharacter
	virtual bool GetIsAttaching() override;                      // AP7BaseCharacter
	virtual void SetOverlappingItem(AP7Item* Item) override;     // IP7PickupInterface
	virtual void AddCoins(const int32 Coins) override;           // IP7PickupInterface
	virtual void AddSouls(const int32 Souls) override;                             // IP7PickupInterface
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; };
	FORCEINLINE EActionState GetActionState() const { return ActionState; };

protected:
	virtual void BeginPlay() override;
	virtual bool CanAttack() override;                                            // AP7BaseCharacter
	virtual void Attack() override;                                               // AP7BaseCharacter
	virtual void OnAttackEndHandle(USkeletalMeshComponent* MeshComp) override;    // AP7BaseCharacter
	virtual void OnBeamTurningHandle(USkeletalMeshComponent* MeshComp) override;  // AP7BaseCharacter
	virtual void OnBeltSnappingHandle(USkeletalMeshComponent* MeshComp) override; // AP7BaseCharacter
	virtual void OnHitReactEndHandle(USkeletalMeshComponent* MeshComp) override;  // AP7BaseCharacter
	void Grab();

private:
	void EquipWeapon(AP7Weapon* Weapon);
	void SetupComponents();
	void OrientToMovement(const bool bOrient) const;
	void AddMappingContext() const;
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Block(bool bBlock);
	void InitOverlayWidget();
	
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
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;
	EActionState ActionState = EActionState::EAS_Unoccupied;
	UPROPERTY()
	UEnhancedInputComponent* EnhancedInputComponent;
	UPROPERTY(VisibleInstanceOnly)
	AP7Item* OverlappingItem;
	UPROPERTY()
	UP7PlayerOverlay* OverlayWidget;
};
