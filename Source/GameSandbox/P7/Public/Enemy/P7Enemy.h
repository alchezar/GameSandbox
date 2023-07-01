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
	virtual void GetHit(const FVector& ImpactPoint) override;

protected:
	virtual void BeginPlay() override;
	void PlayHitReactMontage(const FName& SectionName);

protected:
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	UP7AttributeComponent* Attributes;
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	UP7HealthBarComponent* HealthBarComponent;	
	
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Montage")
	UAnimMontage* HitReactMontage;
};
