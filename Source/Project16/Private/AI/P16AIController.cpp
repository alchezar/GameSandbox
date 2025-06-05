// Copyright © 2025, Ivan Kinder

#include "AI/P16AIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

AP16AIController::AP16AIController()
{
	Blackboard   = CreateDefaultSubobject<UBlackboardComponent>("BlackboardComponent");
	BehaviorTree = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviourTreeComponent");
}

void AP16AIController::BeginPlay()
{
	Super::BeginPlay();
}
