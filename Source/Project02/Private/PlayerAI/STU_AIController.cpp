// Copyright (C) 2023, IKinder

#include "PlayerAI/STU_AIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Component/STU_AIPerceptionComponent.h"
#include "Component/STU_RespawnComponent.h"
#include "PlayerAI/STU_AICharacter.h"

ASTU_AIController::ASTU_AIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	AIPerceptionComponent = CreateDefaultSubobject<USTU_AIPerceptionComponent>("AIPerceptionComponent");
	SetPerceptionComponent(*AIPerceptionComponent);

	RespawnComponent = CreateDefaultSubobject<USTU_RespawnComponent>("RespawnComponent");

	bWantsPlayerState = true;
}

void ASTU_AIController::BeginPlay()
{
	Super::BeginPlay();
}

void ASTU_AIController::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	AActor* AimActor = GetFocusOnActor(); // AIPerceptionComponent->GetClosestEnemy();
	SetFocus(AimActor);
}

void ASTU_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	const ASTU_AICharacter* AICharacter = Cast<ASTU_AICharacter>(InPawn);
	if (AICharacter)
	{
		RunBehaviorTree(AICharacter->BehaviorTreeAsset);
	}
}

AActor* ASTU_AIController::GetFocusOnActor()
{
	if (!GetBlackboardComponent()) return nullptr;
	return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));
}
