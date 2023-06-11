// Copyright (C) 2023, IKinder

#include "ARGameModeBase.h"

#include "EngineUtils.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "P6/AI/ARAICharacter.h"
#include "P6/Component/ARAttributesComponent.h"
#include "P6/Player/ARCharacter.h"

AARGameModeBase::AARGameModeBase()
{}

void AARGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorld()->GetTimerManager().SetTimer(SpawnBotTimer, this, &ThisClass::SpawnBot, SpawnInterval, true);
}

void AARGameModeBase::SpawnBot()
{
	int32 MaxBotNum = 10;
	int32 AliveBotsNum = 0;
	for(TActorIterator<AARAICharacter> It(GetWorld()); It; ++It)
	{
		AARAICharacter* Bot = *It;
		if (!Bot) return;

		const UARAttributesComponent* AttributesComp  = UARAttributesComponent::GetAttributes(Bot);
		if (!AttributesComp || !AttributesComp->GetIsAlive()) return;

		++AliveBotsNum;		
	}
	
	if(DifficultyCurve)
	{
		MaxBotNum = DifficultyCurve->GetFloatValue(GetWorld()->GetTimeSeconds());
	}
	
	if (AliveBotsNum >= MaxBotNum) return;

	
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (!QueryInstance) return;

	QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ThisClass::OnQueryCompleted);
}

void AARGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success) return;
	
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	if (Locations.Num() > 0)
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
	}
}

void AARGameModeBase::KillAll()
{
	for (AARAICharacter* Enemy : TActorRange<AARAICharacter>(GetWorld()))
	{
		UARAttributesComponent* Attributes = UARAttributesComponent::GetAttributes(Enemy);
		Attributes->TryChangeHealth(this, -Attributes->GetHealthMax());
	}
}

void AARGameModeBase::OnPlayerKilled(AActor* Victim, AActor* Killer)
{
	AARCharacter* Player = Cast<AARCharacter>(Victim);
	if (!Player) return;

	// Player->RemoveWidget();

	FTimerHandle RespawnTimer;
	FTimerDelegate RespawnDelegate;
	RespawnDelegate.BindLambda([=]()
	{
		if (AController* Controller = Player->GetController())
		{
			Controller->UnPossess();
			RestartPlayer(Controller);
			// Player->AddWidget();
			// Player->RefreshWidget();
			// Player->RemoveWidget();
		}
	});
	constexpr float RespawnDelay = 2.f;
	GetWorld()->GetTimerManager().SetTimer(RespawnTimer, RespawnDelegate, RespawnDelay, false);
}
