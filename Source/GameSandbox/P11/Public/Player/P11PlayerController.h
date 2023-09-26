// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "P11PlayerController.generated.h"

enum class EP11PlayerSide : uint8;
class AP11HUD;
class UP11StatScoreboard;
class AP11Character;
class UInputAction;
class UInputMappingContext;

UCLASS()
class GAMESANDBOX_API AP11PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void OnPossess(APawn* InPawn) override;
	void SetCharSide(EP11PlayerSide NewSide);
	UFUNCTION(Client, Reliable)
	void Client_ChangeSide();
	UFUNCTION(Server, Reliable)
	void Server_ChangeCharSide(const EP11PlayerSide NewSide);
	UFUNCTION()
	void ChangeCharSide(const EP11PlayerSide NewSide) const;
	UFUNCTION(Server, Unreliable)
	void Server_SendToPlayerControllerGameState(const FString& Sender, const FString& Message);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	void MainMenuInput();
	void ScoreboardInput(const bool bVisible);
	void ChatInput();

protected:
	UPROPERTY(EditAnywhere, Category = "C++ | Input")
	UInputMappingContext* DefaultContext;
	UPROPERTY(EditAnywhere, Category = "C++ | Input")
	UInputAction* MainMenuAction;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	UInputAction* ScoreboardAction;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	UInputAction* ChatAction;
	
private:
	bool bMenuVisible = false;
	UPROPERTY()
	AP11HUD* HUD;
};
