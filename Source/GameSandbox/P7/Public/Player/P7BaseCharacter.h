// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "CharacterTypes.h"
#include "GameFramework/Character.h"
#include "P7/Public/Interface/P7HitInterface.h"
#include "P7BaseCharacter.generated.h"

class AP7Weapon;
class UAnimMontage;
class UP7AttributeComponent;

UCLASS()
class GAMESANDBOX_API AP7BaseCharacter : public ACharacter, public IP7HitInterface
{
	GENERATED_BODY()

public:
	AP7BaseCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Jump() override;
	virtual void Landed(const FHitResult& Hit) override;
	virtual void GetHit(const FVector& ImpactPoint) override;
	virtual bool GetIsAttaching();
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	FORCEINLINE bool GetIsDoubleJump() const { return bDoubleJump; };
	FORCEINLINE UP7AttributeComponent* GetAttributes() const { return Attributes; };
	float GetMovementDirectionAngle();
	void SetAnimSection(const int32 StartSection);

protected:
	virtual void BeginPlay() override;
	virtual void Attack();
	virtual void Die();
	virtual bool CanAttack();
	virtual void OnAttackEndHandle(USkeletalMeshComponent* MeshComp);
	virtual void OnBeamTurningHandle(USkeletalMeshComponent* MeshComp);
	virtual void OnBeltSnappingHandle(USkeletalMeshComponent* MeshComp);

private:
	void InitAnimNotifies();
	void PlayMontageSection(UAnimMontage* Montage);
	void PlayHitReactMontage(const FName& SectionName);
	void DirectionalHitReact(const FVector& ImpactPoint);
	double AngleBetweenVectors(const FVector& Vector1, const FVector& Vector2) const;

protected:	
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	UP7AttributeComponent* Attributes;
	
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Weapon")
	FName HandSocketName = "GripPoint";
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Weapon")
	FName BeltSocketName = "BeltSocket";

#pragma region Montage
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Montage")
	UAnimMontage* AttackMontage{};
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Montage")
	UAnimMontage* EquipMontage{};
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Montage")
	UAnimMontage* HitReactMontage{};
#pragma endregion // Montage

	UPROPERTY(VisibleInstanceOnly)
	AP7Weapon* EquippedWeapon{};

private:
	bool bDoubleJump = false;
	int32 Section = 0;
	FTimerHandle ComboTimer;
};