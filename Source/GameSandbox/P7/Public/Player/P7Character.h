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
	void AddMappingContext() const;
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Grab();
	void OrientToMovement(const bool bOrient) const;

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
#pragma endregion // Input

private:
	UPROPERTY()
	UEnhancedInputComponent* EnhancedInputComponent;
	bool bDoubleJump = false;
	UPROPERTY(VisibleInstanceOnly)
	AP7Item* OverlappingItem;
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;
};
