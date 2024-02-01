// Copyright © 2024, IKinder

#include "P13/Public/Game/P13LobbyGameMode.h"

#include "EngineUtils.h"
#include "Camera/CameraActor.h"
#include "Engine/TargetPoint.h"
#include "GameFramework/PlayerState.h"
#include "P13/Public/Character/P13CharacterBase.h"
#include "P13/Public/Controller/P13LobbyPlayerController.h"
#include "P13/Public/Game/P13GameInstance.h"

AP13LobbyGameMode::AP13LobbyGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	bUseSeamlessTravel = true;
}

void AP13LobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

	InitSpawnPoints();
}

void AP13LobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	InitSpawnPoints();

	if (!NewPlayer->IsA<AP13LobbyPlayerController>())
	{
		return;
	}

	AP13LobbyPlayerController* LobbyPlayerController = StaticCast<AP13LobbyPlayerController*>(NewPlayer);
	check(LobbyPlayerController)
	LobbyPlayerController->OnLogin();
	LobbyPlayerController->UpdateSelectedMapName(LevelName);
	LobbyPlayerControllers.Add(LobbyPlayerController);

	SpawnPlayerPreview(LobbyPlayerController);
}

void AP13LobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	AP13LobbyPlayerController* LobbyPlayerController = Cast<AP13LobbyPlayerController>(Exiting);
	if (!LobbyPlayerController)
	{
		return;
	}

	/* Release occupied target point. */
	ATargetPoint* ReleasedTargetPoint = LobbyPlayerController->ReleaseTargetPoint();
	if (!ReleasedTargetPoint)
	{
		return;
	}

	/* At the existing key, older value will be replaced with the new one. */
	SpawnPointsMap.Add(ReleasedTargetPoint, true);

	/* Release occupied color in the lobby widget. */
	UpdateSelectedColor(DefaultPawnColor, LobbyPlayerController);
}

void AP13LobbyGameMode::Server_LaunchGame_Implementation(const FName InLevelAddress)
{
	GetWorld()->ServerTravel(InLevelAddress.ToString());
}

void AP13LobbyGameMode::UpdateSelectedLevelForAll(const FString& InLevelName)
{
	LevelName = FText::FromString(InLevelName);

	for (const AP13LobbyPlayerController* LobbyController : LobbyPlayerControllers)
	{
		LobbyController->UpdateSelectedMapName(LevelName);
	}
}

void AP13LobbyGameMode::UpdateSelectedColor(const FLinearColor OccupiedColor, const AP13LobbyPlayerController* Occupier)
{
	/* Update occupied color for each controller's hud. */
	for (const AP13LobbyPlayerController* LobbyController : LobbyPlayerControllers)
	{
		LobbyController->Client_UpdateSelectedColorOccupation(OccupiedColor, Occupier->GetOccupiedColor(), Occupier);
	}

	/* Update preview pawn color. */
	if (AP13CharacterBase* CharacterBase = Occupier->GetPawn<AP13CharacterBase>())
	{
		CharacterBase->Multicast_UpdatePlayerColor(OccupiedColor);
	}
}

void AP13LobbyGameMode::SavePlayersColor()
{
	UP13GameInstance* GameInstance = GetGameInstance<UP13GameInstance>();
	check(GameInstance)

	for (const AP13LobbyPlayerController* LobbyController : LobbyPlayerControllers)
	{
		if (!LobbyController)
		{
			continue;
		}
		const AP13CharacterBase* ControlledPawn = LobbyController->GetPawn<AP13CharacterBase>();
		const FLinearColor PlayerColor = ControlledPawn ? ControlledPawn->GetTrueColor() : FLinearColor::White;

		const APlayerState* PlayerState = LobbyController->GetPlayerState<APlayerState>();
		check(PlayerState)

		GameInstance->SavePlayerColor(PlayerState->GetPlayerName(), PlayerColor);
	}
}

void AP13LobbyGameMode::InitSpawnPoints()
{
	if (!SpawnPointsMap.IsEmpty())
	{
		return;
	}

	for (TActorIterator<ATargetPoint> It(GetWorld()); It; ++It)
	{
		ATargetPoint* TargetPoint = *It;
		SpawnPointsMap.Emplace(TargetPoint, true);
	}
}

ATargetPoint* AP13LobbyGameMode::GetFreeSpawnPoint()
{
	for (auto& [TargetPoint, bAvailable] : SpawnPointsMap)
	{
		if (!bAvailable)
		{
			continue;
		}

		bAvailable = false;
		return TargetPoint;
	}
	return nullptr;
}

void AP13LobbyGameMode::SpawnPlayerPreview(AP13LobbyPlayerController* PreviewOwner)
{
	ATargetPoint* TargetPoint = GetFreeSpawnPoint();
	if (!TargetPoint)
	{
		return;
	}

	AP13CharacterBase* CharBase = GetWorld()->SpawnActor<AP13CharacterBase>(CharBaseClass, TargetPoint->GetActorTransform());
	check(CharBase)
	CharBase->CreateDynamicMeshMaterials();

	PreviewOwner->SetPawn(CharBase);
	PreviewOwner->OccupyTargetPoint(TargetPoint);
}

AActor* AP13LobbyGameMode::FindCameraView() const
{
	for (TActorIterator<ACameraActor> It(GetWorld()); It; ++It)
	{
		ACameraActor* CameraActor = *It;
		if (!CameraActor || !CameraActor->ActorHasTag("LobbyView"))
		{
			continue;
		}
		return StaticCast<AActor*>(CameraActor);
	}
	return nullptr;
}
