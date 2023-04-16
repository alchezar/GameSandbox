// Copyright (C) 2023, IKinder

#include "STU_AIPerceptionComponent.h"
#include "GameSandbox/P2/Component/STU_HealthComponent.h"
#include "Perception/AISenseConfig.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Sight.h"

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

AActor* USTU_AIPerceptionComponent::GetClosestEnemy() const
{
	// const FAISenseID SenseID = UAISense::GetSenseID(UAISense_Sight::StaticClass());
	const FAISenseID SenseID = UAISense::GetSenseID<UAISense_Sight>();
	const auto SenseConfig = Cast<UAISenseConfig_Sight>(GetSenseConfig(SenseID));
	
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
		if (HealthComponent && !HealthComponent->IsDead()) // TODO: Check if enemy or not
		{
			const double CurrentDistance = (PerceiveActor->GetActorLocation() - Pawn->GetActorLocation()).Size();
			if (CurrentDistance < BestDistance)
			{
				BestDistance = CurrentDistance;
				BestPawn = PerceiveActor;
			}
		}
	}
	return BestPawn;
}
