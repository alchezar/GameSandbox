// Copyright (C) 2023, IKinder

#include "STU_AIPerceptionComponent.h"
#include "GameSandbox/P2/Component/STU_HealthComponent.h"
#include "Perception/AISenseConfig.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Sight.h"
#include "Player/STU_PlayerState.h"

USTU_AIPerceptionComponent::USTU_AIPerceptionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USTU_AIPerceptionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USTU_AIPerceptionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

AActor* USTU_AIPerceptionComponent::GetClosestEnemy()
{
	// const FAISenseID SenseID = UAISense::GetSenseID(UAISense_Sight::StaticClass());
	const FAISenseID SenseID     = UAISense::GetSenseID<UAISense_Sight>();
	const auto       SenseConfig = Cast<UAISenseConfig_Sight>(GetSenseConfig(SenseID));

	TArray<AActor*> PerceiveActors;
	GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), OUT PerceiveActors);
	if (PerceiveActors.IsEmpty()) return nullptr;

	const AController* Controller = Cast<AController>(GetOwner());
	if (!Controller) return nullptr;

	const APawn* Pawn = Controller->GetPawn();
	if (!Pawn) return nullptr;

	float   BestDistance = SenseConfig->LoseSightRadius; // MAX_FLT;
	AActor* BestPawn     = nullptr;

	for (const auto PerceiveActor : PerceiveActors)
	{
		const auto HealthComponent = PerceiveActor->FindComponentByClass<USTU_HealthComponent>();

		const APawn* PerceivePawn = Cast<APawn>(PerceiveActor);
		const bool   bEnemies     = PerceivePawn && AreEnemies(Controller, PerceivePawn->Controller);

		if (HealthComponent && !HealthComponent->IsDead() && bEnemies)
		{
			const double CurrentDistance = (PerceiveActor->GetActorLocation() - Pawn->GetActorLocation()).Size();
			if (CurrentDistance < BestDistance)
			{
				BestDistance = CurrentDistance;
				BestPawn     = PerceiveActor;
			}
		}
	}
	return BestPawn;
}

bool USTU_AIPerceptionComponent::AreEnemies(const AController* Controller1, const AController* Controller2) const
{
	if (!Controller1 || !Controller2 || Controller1 == Controller2) return false;

	const  ASTU_PlayerState* PlayerState1 = Cast<ASTU_PlayerState>(Controller1->PlayerState);
	const  ASTU_PlayerState* PlayerState2 = Cast<ASTU_PlayerState>(Controller2->PlayerState);

	return PlayerState1 && PlayerState2 && PlayerState1->GetTeamID() != PlayerState2->GetTeamID();
}
