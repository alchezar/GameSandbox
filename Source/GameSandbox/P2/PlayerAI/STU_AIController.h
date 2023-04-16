// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "STU_AIController.generated.h"

class USTU_AIPerceptionComponent;

UCLASS()
class GAMESANDBOX_API ASTU_AIController : public AAIController
{
	GENERATED_BODY()

public:
	ASTU_AIController(const FObjectInitializer& ObjectInitializer);
	
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Component")
	USTU_AIPerceptionComponent* AIPerceptionComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Component")
	FName FocusOnKeyName = "EnemyActor";
		
private:
	AActor* GetFocusOnActor();
};
