// Copyright © 2025, Ivan Kinder

#include "Controller/P17ControllerAI.h"

#include "Project17.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AP17ControllerAI::AP17ControllerAI(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent"))
{
	SenseSight = CreateDefaultSubobject<UAISenseConfig_Sight>("EnemySenseConfigSight");
	SenseSight->DetectionByAffiliation.bDetectEnemies = true;
	SenseSight->DetectionByAffiliation.bDetectFriendlies = false;
	SenseSight->DetectionByAffiliation.bDetectNeutrals = false;
	SenseSight->SightRadius = 1000.f;
	SenseSight->LoseSightRadius = 1500.f;
	SenseSight->PeripheralVisionAngleDegrees = 180.f;

	PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>("EnemyPerceptionComponent");
	PerceptionComp->ConfigureSense(*SenseSight);
	PerceptionComp->SetDominantSense(UAISenseConfig_Sight::StaticClass());
	PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &ThisClass::OnTargetPerceptionUpdatedCallback);

	SetTeamId(1);
}

void AP17ControllerAI::BeginPlay()
{
	Super::BeginPlay();

	UCrowdFollowingComponent* CrowdComp = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent());
	WARN_RETURN_IF(!CrowdComp,);
	CrowdComp->SetCrowdSimulationState(bEnableDetourCrowdAvoidance ? ECrowdSimulationState::Enabled : ECrowdSimulationState::Disabled);
	CrowdComp->SetCrowdAvoidanceQuality(SelectAvoidanceQuality());
	CrowdComp->SetAvoidanceGroup(1);
	CrowdComp->SetGroupsToAvoid(1);
	CrowdComp->SetCrowdCollisionQueryRange(CollisionQueryRange);
}

ETeamAttitude::Type AP17ControllerAI::GetTeamAttitudeTowards(const AActor& Other) const
{
	const APawn* PawnToCheck = Cast<const APawn>(&Other);
	RETURN_IF(!PawnToCheck, ETeamAttitude::Neutral)
	const TScriptInterface<IGenericTeamAgentInterface> TeamInterface = PawnToCheck->GetController();
	RETURN_IF(!TeamInterface, ETeamAttitude::Neutral)

	const bool bSameTeam = TeamInterface->GetGenericTeamId() == GetGenericTeamId();
	return bSameTeam ? ETeamAttitude::Friendly : ETeamAttitude::Hostile;
}

void AP17ControllerAI::OnTargetPerceptionUpdatedCallback(AActor* Actor, FAIStimulus Stimulus)
{
	RETURN_IF(!Stimulus.WasSuccessfullySensed() || !Actor,)
	UBlackboardComponent* BlackboardComp = GetBlackboardComponent();
	RETURN_IF(!BlackboardComp || BlackboardComp->GetValueAsObject("TargetActor") == Actor,)
	BlackboardComp->SetValueAsObject("TargetActor", Actor);
}

void AP17ControllerAI::SetTeamId(const uint8 TeamId)
{
	SetGenericTeamId({TeamId});
}

ECrowdAvoidanceQuality::Type AP17ControllerAI::SelectAvoidanceQuality() const
{
	ECrowdAvoidanceQuality::Type QualityType;
	switch (DetourCrowdAvoidanceQuality)
	// clang-format off
	{
		case 4: QualityType = ECrowdAvoidanceQuality::High;   break;
		case 3: QualityType = ECrowdAvoidanceQuality::Good;   break;
		case 2: QualityType = ECrowdAvoidanceQuality::Medium; break;
		default: QualityType = ECrowdAvoidanceQuality::Low;
	}
	// clang-format on
	return QualityType;
}
