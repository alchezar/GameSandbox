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
	FORCEINLINE bool  GetIsDoubleJump() const { return bDoubleJump; };
	void SetAnimSection(const int32 StartSection);
	float GetMovementDirectionAngle();

protected:
	virtual void BeginPlay() override;
	virtual void Attack();
	virtual void Die();
	virtual bool CanAttack();
	virtual void OnAttackEndHandle(USkeletalMeshComponent* MeshComp);
	virtual void OnBeamTurningHandle(USkeletalMeshComponent* MeshComp);
	virtual void OnBeltSnappingHandle(USkeletalMeshComponent* MeshComp);
	void InitAnimNotifies();
	void PlayAttackMontage();
	void PlayHitReactMontage(const FName& SectionName);
	void DirectionalHitReact(const FVector& ImpactPoint);
	double AngleBetweenVectors(const FVector& Vector1, const FVector& Vector2) const;

protected:	
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component") // enemy
	UP7AttributeComponent* Attributes;
	
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Weapon") // char
	FName HandSocketName = "GripPoint";
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Weapon") // char
	FName BeltSocketName = "BeltSocket";

#pragma region Montage
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Montage")
	UAnimMontage* AttackMontage{};
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Montage")
	UAnimMontage* EquipMontage{};
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Montage") // enemy
	UAnimMontage* HitReactMontage{};
#pragma endregion // Montage

	UPROPERTY(VisibleInstanceOnly)
	AP7Weapon* EquippedWeapon{};

private:
	bool bDoubleJump = false; // char
	int32 Section = 0;
	FTimerHandle ComboTimer;
};
