// Copyright (C) 2023, IKinder

#include "STU_Context_Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"

void USTU_Context_Enemy::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	AActor* QueryOwner = Cast<AActor>(QueryInstance.Owner.Get());
	const UBlackboardComponent* Blackboard = UAIBlueprintHelperLibrary::GetBlackboard(QueryOwner);
	if (!Blackboard) return;

	const AActor* QueryActor = Cast<AActor>(Blackboard->GetValueAsObject(EnemyActorKeyName));	
	UEnvQueryItemType_Actor::SetContextHelper(ContextData, QueryActor);
}
