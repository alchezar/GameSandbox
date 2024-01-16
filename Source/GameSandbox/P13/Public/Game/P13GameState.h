// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "P13GameState.generated.h"

UENUM(BlueprintType)
enum class EP13LevelPhase : uint8
{
	Phase1,
	Phase2,
	Phase3,
	Phase4,
	Phase5,
	Phase6,
	MAX UMETA(Hidden)
};

UCLASS()
class GAMESANDBOX_API AP13GameState : public AGameState
{
	GENERATED_BODY()
	
	/* ------------------------------- Super ------------------------------- */
public:
	AP13GameState();

protected:
	virtual void BeginPlay() override;

	/* ------------------------------- This -------------------------------- */
public:
	FORCEINLINE EP13LevelPhase GetLastLevelPhase() const { return LevelPhase; }
	void GoToNextPhase();
	
	/* ----------------------------- Variables ----------------------------- */
protected:
	UPROPERTY(EditAnywhere, Category = "C++")
	EP13LevelPhase StartLevelPhase = EP13LevelPhase::Phase1;
	
private:
	EP13LevelPhase LevelPhase = EP13LevelPhase::Phase1;
};
