// Copyright (C) 2023, IKinder

#include "ARGameModeBase.h"
#include "ARSaveGame.h"
#include "EngineUtils.h"
#include "Engine/AssetManager.h"
#include "Engine/DataTable.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "P6/AI/ARAICharacter.h"
#include "P6/Component/ARAbilityComponent.h"
#include "P6/Component/ARAttributesComponent.h"
#include "P6/Interface/ARGameplayInterface.h"
#include "P6/Player/ARCharacter.h"
#include "P6/Player/ARPlayerController.h"
#include "P6/Player/ARPlayerState.h"
#include "P6/Util/ARMonsterData.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

DEFINE_LOG_CATEGORY_STATIC(LogARGameModeBase, All, All);

AARGameModeBase::AARGameModeBase()
{
	PlayerStateClass = AARPlayerState::StaticClass();
	PlayerControllerClass = AARPlayerController::StaticClass();
}

void AARGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	
	FString SelectedSaveSlot = UGameplayStatics::ParseOption(Options, "SaveGame");
	if (SelectedSaveSlot.Len() > 0)
	{
		SlotName = SelectedSaveSlot;
	}
	LoadSaveGame();
}

void AARGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorld()->GetTimerManager().SetTimer(SpawnBotTimer, this, &ThisClass::SpawnBot, SpawnInterval, true);
}

void AARGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	if (AARPlayerState* PlayerState = NewPlayer->GetPlayerState<AARPlayerState>())
	{
		PlayerState->LoadPlayerState(CurrentSaveGame);
	}
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}

void AARGameModeBase::SpawnBot()
{
	int32 AliveBotsNum = 0;
	for (TActorIterator<AARAICharacter> It(GetWorld()); It; ++It)
	{
		AARAICharacter* Bot = *It;
		if (!Bot) return;

		const UARAttributesComponent* AttributesComp = UARAttributesComponent::GetAttributes(Bot);
		if (!AttributesComp || !AttributesComp->GetIsAlive()) return;

		++AliveBotsNum;
	}

	if (DifficultyCurve)
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
	if (Locations.IsEmpty()) return; 
	
	if (bUseTable && MonsterTable)
	{
		TArray<FMonsterInfoRow*> Rows;
		MonsterTable->GetAllRows("", Rows);
		const int32 RandomRange = FMath::RandRange(0, Rows.Num() - 1);
		const FMonsterInfoRow* SelectedRow = Rows[RandomRange];
		
		const TArray<FName> Bundles;
		const FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(
			this, &ThisClass::OnMonsterLoaded, SelectedRow->MonsterId, Locations[0], SelectedRow->TeamColor);
		
		UAssetManager* Manager = UAssetManager::GetIfValid();
		if (!Manager) return;
		Manager->LoadPrimaryAsset(SelectedRow->MonsterId, Bundles, Delegate);
	}
	else
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
	}
}

void AARGameModeBase::OnMonsterLoaded(const FPrimaryAssetId LoadedId, const FVector SpawnLocation, const FLinearColor Color)
{
	const UAssetManager* Manager = UAssetManager::GetIfValid();
	if (!Manager) return;
	UARMonsterData* MonsterData = Manager->GetPrimaryAssetObject<UARMonsterData>(LoadedId);
	if (!MonsterData) return;
	
	AARAICharacter* Bot = GetWorld()->SpawnActor<AARAICharacter>(MonsterData->MonsterClass, SpawnLocation, FRotator::ZeroRotator);
	if (!Bot) return;		
	Bot->SetTeamColor(Color);
		
	UARAbilityComponent* AbilityComp = Bot->FindComponentByClass<UARAbilityComponent>();
	if (!AbilityComp) return;
	for (const TSubclassOf<UARAbility> Ability : MonsterData->Abilities)
	{
		AbilityComp->AddAbility(Bot, Ability);
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
	if (const AARCharacter* Player = Cast<AARCharacter>(Victim))
	{
		FTimerHandle RespawnTimer;
		FTimerDelegate RespawnDelegate;
		RespawnDelegate.BindLambda([=]()
		{
			if (AController* Controller = Player->GetController())
			{
				Controller->UnPossess();
				RestartPlayer(Controller);
			}
		});
		constexpr float RespawnDelay = 2.f;
		GetWorld()->GetTimerManager().SetTimer(RespawnTimer, RespawnDelegate, RespawnDelay, false);
	}

	if (const APawn* KillerPawn = Cast<APawn>(Killer))
	{
		AARPlayerState* PlayerState = KillerPawn->GetPlayerState<AARPlayerState>();
		if (!PlayerState) return;

		PlayerState->AddCredits(CreditsPerKill);
	}
}

void AARGameModeBase::WriteSaveGame()
{
	/* Iterate all player states */
	for (TObjectPtr<APlayerState> State : GameState->PlayerArray) 
	{
		if (AARPlayerState* PlayerState = Cast<AARPlayerState>(State))
		{
			PlayerState->SavePlayerState(CurrentSaveGame);
			break; // Single player only at this point
		}
	}

	/* Iterate the entire world of actors to find only "gameplay actors" */
	CurrentSaveGame->SavedActors.Empty(); 
	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (!Actor->Implements<UARGameplayInterface>()) continue;

		FActorSaveData ActorData = {Actor->GetName(), Actor->GetActorTransform()};
		FMemoryWriter MemoryWriter(ActorData.ByteData);
		
		FObjectAndNameAsStringProxyArchive Ar(MemoryWriter, true); // Pass the array to fill with data from Actor
		Ar.ArIsSaveGame = true;                                    // Find only variables with UPROPERTY(SaveGame)
		Actor->Serialize(Ar);                                      // Convert actors SaveGame UPROPERTY to binary array
				
		CurrentSaveGame->SavedActors.Add(ActorData);
	}

	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
}

void AARGameModeBase::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		CurrentSaveGame = Cast<UARSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		if (!CurrentSaveGame) return;

		/* Find matches and set transform */
		for (FActorIterator It(GetWorld()); It; ++It)
		{
			AActor* Actor = *It;
			if (!Actor->Implements<UARGameplayInterface>()) continue;

			for (FActorSaveData ActorData : CurrentSaveGame->SavedActors)
			{
				if (ActorData.Name == Actor->GetName())
				{
					FMemoryReader MemoryReader(ActorData.ByteData);
					FObjectAndNameAsStringProxyArchive Ar(MemoryReader, true); // Get data from the array to fill Actor
					Ar.ArIsSaveGame = true;                                    // Find Only variables with UPROPERTY(SaveGame)
					Actor->Serialize(Ar);                                      // Convert binary array back into actor`s variables
					IARGameplayInterface::Execute_OnActorLoaded(Actor);

					Actor->SetActorTransform(ActorData.Transform);
					break;
				}
			}
		}
	}
	else
	{
		CurrentSaveGame = Cast<UARSaveGame>(UGameplayStatics::CreateSaveGameObject(UARSaveGame::StaticClass()));
		UE_LOG(LogARGameModeBase, Log, TEXT("Created new SaveGame data"));
	}
}