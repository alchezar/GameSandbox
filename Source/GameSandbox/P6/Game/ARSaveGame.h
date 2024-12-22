// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ARSaveGame.generated.h"

USTRUCT()
struct FActorSaveData
{
	GENERATED_BODY()

	UPROPERTY()
	FString Name;
	UPROPERTY()
	FTransform Transform;
	UPROPERTY()
	TArray<uint8> ByteData;
};

UCLASS()
class GAMESANDBOX_API UARSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int32 Credits;
	UPROPERTY()
	TArray<FActorSaveData> SavedActors;
};
