// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LS_BaseCharacter.generated.h"

class ALS_LightSaber;
class UAnimMontage;
class USkeletalMeshComponent;

USTRUCT(BlueprintType)
struct FTakeLightSaber
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Montage")
	UAnimMontage* Get;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Montage")
	UAnimMontage* Put;
};

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
	bool GetIsWeaponTaken() const;
	ALS_LightSaber* GetCurrentSaber() const;
	
	void Attack();
	void TakeWeapon();

protected:
	virtual void BeginPlay() override;

private:
	void SpawnWeapon();
	void SetTeamColor();
	void InitAnimationNotifyStates();

	void OnAttackBeginHandle(USkeletalMeshComponent* MeshComp);
	void OnAttackEndHandle(USkeletalMeshComponent* MeshComp);
	void OnSaberActivationHandle(USkeletalMeshComponent* MeshComp);
	void OnSaberVisibilityHandle(USkeletalMeshComponent* MeshComp);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Body")
	FName TeamColorParameterName = "PaintColor";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Body")
	FLinearColor TeamColor = FLinearColor(0.5f, 0.f, 1.f, 1.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Saber")
	TSubclassOf<ALS_LightSaber> LightSaberClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Saber")
	FName SocketName = "GripPoint";
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Kinder | Saber")
	bool bAttacking = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Health")
	float MaxHealth = 100.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Kinder | Health")
	float Health = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Montage")
	TArray<UAnimMontage*> AttackMontageArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Montage")
	FTakeLightSaber TakeLightSaber;
	
private:
	UPROPERTY()
	ALS_LightSaber* CurrentSaber;
	bool bDoubleJump = false;
	bool bTaken = true;
	int32 CompoCount = 0;
	FTimerHandle AttackTimer;
};
