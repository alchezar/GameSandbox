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
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void GetHit(const FVector& HitterLocation) override;
	virtual bool GetIsAttaching();
	FORCEINLINE bool GetIsDoubleJump() const { return bDoubleJump; };
	FORCEINLINE bool GetIsBlocked() const { return bBlocking; };
	FORCEINLINE UP7AttributeComponent* GetAttributes() const { return Attributes; };
	float GetMovementDirectionAngle();
	void SetAnimSection(const int32 StartSection);
	void SetIsBlocked(const bool bBlock);

protected:
	virtual void BeginPlay() override;
	virtual void Die();
	virtual void Attack();
	virtual bool CanAttack();
	virtual void OnAttackEndHandle(USkeletalMeshComponent* MeshComp);
	virtual void OnBeamTurningHandle(USkeletalMeshComponent* MeshComp);
	virtual void OnBeltSnappingHandle(USkeletalMeshComponent* MeshComp);
	virtual void OnHitReactEndHandle(USkeletalMeshComponent* MeshComp);
	
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
	UAnimMontage* AttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Montage")
	UAnimMontage* EquipMontage;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Montage")
	UAnimMontage* HitReactMontage;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Montage")
	UAnimMontage* BlockingHitMontage;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Montage")
	UAnimMontage* RollMontage;
#pragma endregion // Montage

	UPROPERTY(VisibleInstanceOnly)
	AP7Weapon* EquippedWeapon;

private:
	bool bDoubleJump = false;
	bool bBlocking = false;
	int32 Section = 0;
	FTimerHandle ComboTimer;
};
