// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "STU_BaseCharacter.h"
#include "STU_AICharacter.generated.h"

class UBehaviorTree;

UCLASS()
class GAMESANDBOX_API ASTU_AICharacter : public ASTU_BaseCharacter
{
	GENERATED_BODY()

public:
	ASTU_AICharacter(const FObjectInitializer& ObjectInitializer);

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | AI")
	UBehaviorTree* BehaviorTreeAsset;

protected:
	virtual void BeginPlay() override;

public:
};
