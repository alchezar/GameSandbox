// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "CharacterTypes.h"
#include "Interface/P7PickupInterface.h"
#include "Player/P7BaseCharacter.h"
#include "P7Character.generated.h"

class UP7WallRunComponent;
struct FInputActionValue;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class AP7Item;
class UP7PlayerOverlay;

UCLASS()
class PROJECT07_API AP7Character : public AP7BaseCharacter, public IP7PickupInterface
{
	GENERATED_BODY()

public:
	AP7Character();	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Jump() override;
	virtual void GetHit(const FVector& HitterLocation) override; // AP7BaseCharacter
	virtual bool GetIsAttaching() override;                      // AP7BaseCharacter
	virtual void SetOverlappingItem(AP7Item* Item) override;     // IP7PickupInterface
	virtual void AddCoins(const int32 Coins) override;           // IP7PickupInterface
	virtual void AddSouls(const int32 Souls) override;           // IP7PickupInterface
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; };
	FORCEINLINE EActionState GetActionState() const { return ActionState; };
	FORCEINLINE UP7WallRunComponent* GetWallRunComponent() const { return WallRunComponent; }

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
	void Stand(const FInputActionValue& Value);
	void Roll();
	void Block(bool bBlock);
	void Run(bool bRun);
	void InitOverlayWidget();
	void Climb();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	USpringArmComponent* CameraBoom;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	UCameraComponent* ViewCamera;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	UP7WallRunComponent* WallRunComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | Moving")
	float StaminaCost = 20.f;

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
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	UInputAction* RollAction;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	UInputAction* RunAction;
#pragma endregion // Input

	UPROPERTY(EditDefaultsOnly, Category = "C++ | Montage")
	UAnimMontage* ClimbMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Debug")
	bool bDebug = false;

private:
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;
	EActionState ActionState = EActionState::EAS_Unoccupied;
	FVector2D LastMovementVector = FVector2D::ZeroVector;
	UPROPERTY()
	UEnhancedInputComponent* EnhancedInputComponent;
	UPROPERTY(VisibleInstanceOnly)
	AP7Item* OverlappingItem;
	UPROPERTY()
	UP7PlayerOverlay* OverlayWidget;
};
