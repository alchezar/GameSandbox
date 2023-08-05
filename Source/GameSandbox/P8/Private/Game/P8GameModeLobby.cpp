// Copyright (C) 2023, IKinder

#include "P8/Public/Game/P8GameModeLobby.h"

#include "P8/Public/Game/P8GameInstance.h"

AP8GameModeLobby::AP8GameModeLobby()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AP8GameModeLobby::BeginPlay()
{
	Super::BeginPlay();
}

void AP8GameModeLobby::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP8GameModeLobby::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	if (++NumberOfPlayers >= 2)
	{
		bUseSeamlessTravel = true;
		const UP8GameInstance* GameInstance = Cast<UP8GameInstance>(GetGameInstance());
		if (!GameInstance || !GetWorld()) return;
		
		constexpr float StartDelay = 5.f;
		GetWorld()->GetTimerManager().SetTimer(GameStartTimer, this, &ThisClass::TravelToGame, StartDelay);
	}
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Number of players: %d"), NumberOfPlayers));
}

void AP8GameModeLobby::TravelToGame() const
{
	UP8GameInstance* GameInstance = Cast<UP8GameInstance>(GetGameInstance());
	if (!GameInstance || !GetWorld()) return;

	GameInstance->StartSession();
	GetWorld()->ServerTravel(GameInstance->GetGameURL() + "?listen");
}

void AP8GameModeLobby::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	--NumberOfPlayers;
}

