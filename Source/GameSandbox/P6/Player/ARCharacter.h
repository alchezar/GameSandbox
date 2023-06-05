// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ARCharacter.generated.h"

struct FInputActionValue;
class UARInteractionComponent;
class AARMagicProjectile;
class UInputAction;
class UInputMappingContext;
class UCameraComponent;
class USpringArmComponent;
class UAnimMontage;

UCLASS()
class GAMESANDBOX_API AARCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AARCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& InputValue);
	void Look(const FInputActionValue& InputValue);
	void Fire();
	void SetTeamColor() const;

private:
	void AddMappingContext() const;
	void ConstructComponents();
	void PrimaryInteract();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | Component")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | Component")
	UCameraComponent* CameraComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | Component")
	UARInteractionComponent* InteractionComponent;

#pragma region Input
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | Input")
	UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | Input")
	UInputAction* MoveAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | Input")
	UInputAction* LookAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | Input")
	UInputAction* JumpAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | Input")
	UInputAction* FireAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | Input")
	UInputAction* InteractAction;
#pragma endregion // Input

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | Effect")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Body")
	FName TeamColorParameterName = "PaintColor";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Body")
	FLinearColor TeamColor = FLinearColor(1.f, 1.f, 0.f, 1.f);
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | Body")
	FName HandSocketName = "GripPoint";

#pragma region Montage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Montage")
	UAnimMontage* AttackMontage;
#pragma endregion // Montage
};
