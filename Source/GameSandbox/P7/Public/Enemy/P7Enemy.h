// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "P7/Public/Interface/P7HitInterface.h"
#include "P7/Public/Player/CharacterTypes.h"
#include "P7Enemy.generated.h"

class UPawnSensingComponent;
class AAIController;
class UP7HealthBarComponent;
class UP7AttributeComponent;

UCLASS()
class GAMESANDBOX_API AP7Enemy : public ACharacter, public IP7HitInterface
{
	GENERATED_BODY()

public:
	AP7Enemy();
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void GetHit(const FVector& ImpactPoint) override;

protected:
	virtual void BeginPlay() override;
	void PlayHitReactMontage(const FName& SectionName);
	UFUNCTION()
	void OnSeePawnHandle(APawn* Pawn);
	
private:
	void DirectionalHitReact(const FVector& ImpactPoint);
	void Die();
	bool InTargetRange(const AActor* Target, const float Radius);
	void ChangeTarget(EEnemyState NewState, AActor* NewTarget);
	void CheckCombatTarget();
	void CheckPatrolTarget();
	void ChoosePatrolTarget();
	void PatrolTimerFinished();
	void MoveToTarget(AActor* Target);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	UP7AttributeComponent* Attributes;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	UP7HealthBarComponent* HealthBarComponent;	
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	UPawnSensingComponent* PawnSensing;
	
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Montage")
	UAnimMontage* HitReactMontage;

	
	UPROPERTY(EditAnywhere, Category = "C++ | Moving")
	TArray<AActor*> PatrolTargets;
	UPROPERTY(EditAnywhere, Category = "C++ | Moving")
	FP7PatrolTime PatrolTime;
	UPROPERTY(EditAnywhere, Category = "C++ | Moving")
	float PatrolRadius = 200.f;	
	UPROPERTY(EditAnywhere, Category = "C++ | Moving")
	float CombatRadius = 500.f;
	UPROPERTY(EditAnywhere, Category = "C++ | Moving")
	float AttackRadius = 150.f;
	
private:
	UPROPERTY()
	AActor* PatrolTarget;
	UPROPERTY()
	AActor* CombatTarget;
	FTimerHandle PatrolTimer;
	UPROPERTY()
	AAIController* EnemyController;
	EEnemyState EnemyState = EES_Patrolling;
};
