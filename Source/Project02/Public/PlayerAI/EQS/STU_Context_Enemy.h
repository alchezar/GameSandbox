// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "STU_Context_Enemy.generated.h"

UCLASS()
class PROJECT02_API USTU_Context_Enemy : public UEnvQueryContext
{
	GENERATED_BODY()

public:
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | AI")
	FName EnemyActorKeyName = "EnemyActor";
};
