// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ER_GameModeBaseStart.generated.h"

class UER_MainMenu;

UCLASS()
class PROJECT03_API AER_GameModeBaseStart : public AGameModeBase
{
	GENERATED_BODY()

public:
	AER_GameModeBaseStart();
	virtual void Tick(float DeltaTime) override;

	FName GetGameLevelName() const;

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Kinder | Widget")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kinder | Game")
	FName GameLevelName = "EndlessRunner";

private:
	UPROPERTY()
	UER_MainMenu* MainMenu;
};
