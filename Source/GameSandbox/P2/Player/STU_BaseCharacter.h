// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "STU_BaseCharacter.generated.h"

class USTU_HealthComponent;
class USTU_WeaponComponent;
class USoundCue;

UCLASS()
class GAMESANDBOX_API ASTU_BaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASTU_BaseCharacter(const FObjectInitializer& ObjectInitializer);
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable, Category = "Kinder | Input")
	virtual bool GetIsRunning() const;
	void SetPlayerColor(const FLinearColor& NewTeamColor);
	UFUNCTION(BlueprintCallable, Category = "Kinder | Movement")
	float GetMovementDirection() const;

protected:
	virtual void BeginPlay() override;
	virtual void OnDeathHandle();
	virtual void OnHealthChangedHandle(float Health, float HealthDelta);

private:
	UFUNCTION()
	void LandedHandle(const FHitResult& Hit);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Components")
	USTU_HealthComponent* HealthComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Components")
	USTU_WeaponComponent* WeaponComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Components")
	TArray<UAnimMontage*> DeathAnimations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Damage")
	float LifeSpanOnDeath = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Damage")
	FVector2D LandedDamageVelocity = FVector2D(900.f, 1200.f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Damage")
	FVector2D LandedDamage = FVector2D(10.f, 100.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Style")
	FLinearColor TeamColor = FLinearColor::White;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Style")
	FName MaterialColorName = "Color";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Sound")
	USoundCue* DeathSound;
};
