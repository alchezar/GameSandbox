// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "P7/Public/Interface/P7HitInterface.h"
#include "P7Enemy.generated.h"

class UP7HealthBarComponent;
class UP7AttributeComponent;

UCLASS()
class GAMESANDBOX_API AP7Enemy : public ACharacter, public IP7HitInterface
{
	GENERATED_BODY()

public:
	AP7Enemy();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void GetHit(const FVector& ImpactPoint) override;

protected:
	virtual void BeginPlay() override;
	void PlayHitReactMontage(const FName& SectionName);

private:
	void DirectionalHitReact(const FVector& ImpactPoint);
	void Die();

protected:
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	UP7AttributeComponent* Attributes;
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	UP7HealthBarComponent* HealthBarComponent;	
	
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Montage")
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere, Category = "C++ | Combat")
	float CombatRadius = 500.f;

private:
	UPROPERTY()
	AActor* CombatTarget;
};
