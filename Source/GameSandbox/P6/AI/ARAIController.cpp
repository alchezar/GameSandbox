// Copyright (C) 2023, IKinder

#include "ARAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

AARAIController::AARAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AARAIController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(BehaviorTree);

	APawn* OurPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!OurPawn) return;

	UBlackboardComponent* BlackboardComp = GetBlackboardComponent();
	if (!BlackboardComp) return;
	
	BlackboardComp->SetValueAsVector("MoveToLocation", OurPawn->GetActorLocation());
	BlackboardComp->SetValueAsObject("TargetActor", OurPawn);
}

void AARAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

