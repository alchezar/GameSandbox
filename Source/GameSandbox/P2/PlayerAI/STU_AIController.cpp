// Copyright (C) 2023, IKinder

#include "STU_AIController.h"
#include "STU_AICharacter.h"
#include "GameSandbox/P2/Component/STU_AIPerceptionComponent.h"

ASTU_AIController::ASTU_AIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	AIPerceptionComponent = CreateDefaultSubobject<USTU_AIPerceptionComponent>("AIPerceptionComponent");
	SetPerceptionComponent(*AIPerceptionComponent);
}

void ASTU_AIController::BeginPlay()
{
	Super::BeginPlay();
}

void ASTU_AIController::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	AActor* AimActor = AIPerceptionComponent->GetClosestEnemy();
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
