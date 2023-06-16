// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ARCharacter.generated.h"

class UARAbility;
class UARAbilityComponent;
class UARAttributesComponent;
struct FInputActionValue;
class UARInteractionComponent;
class AARProjectileMagic;
class UInputAction;
class UInputMappingContext;
class UCameraComponent;
class USpringArmComponent;
class UAnimMontage;
class UUserWidget;

UCLASS()
class GAMESANDBOX_API AARCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AARCharacter();
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual FVector GetPawnViewLocation() const override;

	UARAttributesComponent* GetAttributesComp() const;
	UCameraComponent* GetCameraComp() const;
	
protected:
	virtual void BeginPlay() override;
	
	void Move(const FInputActionValue& InputValue);
	void Look(const FInputActionValue& InputValue);
	void Fire();
	void SetTeamColor() const;
	void SprintStart();
	void SprintStop();
	void Parry();

private:
	void AddMappingContext() const;
	void ConstructComponents();
	void PrimaryInteract();
	void ExposeAbilities();
	UFUNCTION()
	void OnHealthChangedHandle(AActor* InstigatorActor, UARAttributesComponent* OwningComp, float NewHealth, float Delta);
	UFUNCTION()
	void OnDeadHandle(AActor* DeadActor);
	UFUNCTION(Exec)
	void HealSelf(float Amount = 100.f);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | Component")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | Component")
	UCameraComponent* CameraComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | Component")
	UARInteractionComponent* InteractionComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | Component")
	UARAttributesComponent* AttributeComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | Component")
	UARAbilityComponent* AbilityComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Body")
	FName TeamColorParameterName = "PaintColor";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Body")
	FLinearColor TeamColor = FLinearColor(1.f, 1.f, 0.f, 1.f);

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
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | Input")
	UInputAction* SprintAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | Input")
	UInputAction* ParryAction;
#pragma endregion // Input

#pragma region Ability
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Ability")
	TArray<TSubclassOf<UARAbility>> AbilityClasses;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Ability")
	FName AttackTagName = "PrimaryAttack";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Ability")
	FName SprintTagName = "SprintAbility";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Ability")
	FName ParryTagName = "ParryAbility";
#pragma endregion  //Ability

private:
	UPROPERTY()
	UUserWidget* HUD;
};
