// Copyright (C) 2023, IKinder

#include "ARAICharacter.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"

AARAICharacter::AARAICharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");
}

void AARAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &ThisClass::OnPawnSeen);
	
}

void AARAICharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AARAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AARAICharacter::OnPawnSeen(APawn* Pawn)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (!AIController) return;

	UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();
	if (!Blackboard) return;
	
	Blackboard->SetValueAsObject("TargetActor", Pawn);
	DrawDebugString(GetWorld(), Pawn->GetActorLocation(), FString::Printf(TEXT("See you")), nullptr, FColor::Green, 2.f);
}
