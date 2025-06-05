// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "P7/Public/Player/P7BaseCharacter.h"
// #include "P7/Public/Interface/P7HitInterface.h"
#include "P7/Public/Player/CharacterTypes.h"
#include "P7Enemy.generated.h"

class AP7Soul;
class AAIController;
class UP7HealthBarComponent;
#if 0
class UPawnSensingComponent;
#endif

UCLASS()
class GAMESANDBOX_API AP7Enemy : public AP7BaseCharacter
{
	GENERATED_BODY()

public:
	AP7Enemy();
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Destroyed() override;
	virtual void GetHit(const FVector& HitterLocation) override;
	virtual bool GetIsAttaching() override;
	FORCEINLINE FColor GetTeamColor() const { return TeamColor; };

protected:
	virtual void BeginPlay() override;
	virtual void Die() override;
	virtual void Attack() override;
	virtual bool CanAttack() override;
	virtual void OnAttackEndHandle(USkeletalMeshComponent* MeshComp) override;
	virtual void OnHitReactEndHandle(USkeletalMeshComponent* MeshComp) override;
	UFUNCTION()
	void OnSeePawnHandle(APawn* Pawn);
	
private:
	void InitializeEnemy();
	void SpawnWeapon();
	/* AI Behavior */
	bool InsideTargetRadius(const AActor* Target, const float Radius) const;
	void MoveToTarget(AActor* Target);
	void NewTargetBehavior(EEnemyState NewState, AActor* NewTarget);
	void CheckCombatTarget();
	void CheckPatrolTarget();
	void ChoosePatrolTarget();
	void PatrolTimerFinished();
	void LoseInterest();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	UP7HealthBarComponent* HealthBarComponent;
#if 0
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	UPawnSensingComponent* PawnSensing;
#endif
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	TSubclassOf<AP7Soul> SoulClass;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Weapon")
	TSubclassOf<AP7Weapon> WeaponClass;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Weapon")
	FColor TeamColor = FColor::Red;	
	UPROPERTY(EditAnywhere, Category = "C++ | Moving")
	TArray<AActor*> PatrolTargets;
	UPROPERTY(EditAnywhere, Category = "C++ | Moving")
	FP7PatrolTime PatrolTime;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | Moving", meta = (Units = "Centimeters"))
	float PatrolAcceptRadius = 200.f;	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | Moving", meta = (Units = "Centimeters"))
	float CombatRadius = 1000.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | Moving", meta = (Units = "Centimeters"))
	float AttackRadius = 150.f;
	UPROPERTY(EditAnywhere, Category = "C++ | Moving", meta = (Units = "CentimetersPerSecond"))
	float PatrolSpeed = 300.f;
	UPROPERTY(EditAnywhere, Category = "C++ | Moving", meta = (Units = "CentimetersPerSecond"))
	float ChasingSpeed = 600.f;
	
private:
	EEnemyState EnemyState = EES_Patrolling;
	FTimerHandle PatrolTimer;
	FTimerHandle AttackTimer;
	UPROPERTY()
	AActor* PatrolTarget;
	UPROPERTY()
	AActor* CombatTarget;
	UPROPERTY()
	AAIController* EnemyController;
};
