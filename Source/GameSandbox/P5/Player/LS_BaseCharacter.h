// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LS_BaseCharacter.generated.h"

class ALS_LightSaber;
class UAnimMontage;

UCLASS()
class GAMESANDBOX_API ALS_BaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ALS_BaseCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void Landed(const FHitResult& Hit) override;

	virtual void Jump() override;
	bool GetIsDead();
	bool GetIsDoubleJump() const;
	void Attack();

protected:
	virtual void BeginPlay() override;

private:
	void SpawnWeapon();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Saber")
	TSubclassOf<ALS_LightSaber> LightSaberClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Saber")
	FName SocketName = "GripPoint";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Saber")
	FLinearColor SaberColor = FLinearColor(0.5f, 0.f, 1.f, 1.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Health")
	float MaxHealth = 100.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Kinder | Health")
	float Health = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Attack")
	TArray<UAnimMontage*> AttackMontageArray;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Kinder | Health")
	bool bAttacking = false;

private:
	UPROPERTY()
	ALS_LightSaber* CurrentSaber;
	bool bDoubleJump = false;
};
