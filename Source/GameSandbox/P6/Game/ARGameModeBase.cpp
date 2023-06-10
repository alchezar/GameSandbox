// Copyright (C) 2023, IKinder

#include "ARGameModeBase.h"

#include "EngineUtils.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "P6/AI/ARAICharacter.h"
#include "P6/Component/ARAttributesComponent.h"

AARGameModeBase::AARGameModeBase()
{}

void AARGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorld()->GetTimerManager().SetTimer(SpawnBotTimer, this, &ThisClass::SpawnBot, SpawnInterval, true);
}

void AARGameModeBase::SpawnBot()
{
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (!QueryInstance) return;

	QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ThisClass::OnQueryCompleted);
}

void AARGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success) return;

	int32 MaxBotNum = 10;
	int32 AliveBotsNum = 0;
	for(TActorIterator<AARAICharacter> It(GetWorld()); It; ++It)
	{
		const AARAICharacter* Bot = *It;
		if (!Bot) return;

		const UARAttributesComponent* AttributesComp  = Bot->FindComponentByClass<UARAttributesComponent>();
		if (!AttributesComp || !AttributesComp->GetIsAlive()) return;

		++AliveBotsNum;		
	}
	
	if(DifficultyCurve)
	{
		MaxBotNum = DifficultyCurve->GetFloatValue(GetWorld()->GetTimeSeconds());
	}
	
	if (AliveBotsNum >= MaxBotNum) return;

	
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	if (Locations.Num() > 0)
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
	}
}
