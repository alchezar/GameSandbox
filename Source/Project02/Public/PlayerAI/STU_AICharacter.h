// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Player/STU_BaseCharacter.h"
#include "STU_AICharacter.generated.h"

class UWidgetComponent;
class UBehaviorTree;

UCLASS()
class PROJECT02_API ASTU_AICharacter : public ASTU_BaseCharacter
{
	GENERATED_BODY()

public:
	ASTU_AICharacter(const FObjectInitializer& ObjectInitializer);

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void OnHealthChangedHandle(float Health, float HealthDelta) override;

protected:
	virtual void BeginPlay() override;
	virtual void OnDeathHandle() override;
	void UpdateHealthWidgetVisibility();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | AI")
	UBehaviorTree* BehaviorTreeAsset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | AI", meta = (Units = "cm"))
	float HealthVisibilityDistance = 1000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Components")
	UWidgetComponent* HealthWidgetComponent;

protected:
	FTimerHandle HealthWidgetTimer;
};
