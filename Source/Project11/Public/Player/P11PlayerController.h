// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UI/Chat/P11Chat.h"
#include "P11PlayerController.generated.h"

enum class EP11PlayerSide : uint8;
class AP11HUD;
class UP11StatScoreboard;
class AP11Character;
class UInputAction;
class UInputMappingContext;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FP11OnMessageSendToGameModeHandle, const FP11MessageInfo&, MessageInfo);

UCLASS()
class PROJECT11_API AP11PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void OnPossess(APawn* InPawn) override;
	void AfterCloseChat();
	void SetCharSide(EP11PlayerSide NewSide);
	UFUNCTION(Client, Reliable)
	void Client_ChangeSide();
	UFUNCTION(Server, Reliable)
	void Server_ChangeCharSide(const EP11PlayerSide NewSide);
	UFUNCTION()
	void ChangeCharSide(const EP11PlayerSide NewSide) const;
	UFUNCTION(Server, Unreliable)
	void Server_SendToPlayerControllerGameState(const FP11MessageInfo& MessageInfo);
	UFUNCTION(Server, Unreliable)
	void Server_SendToPlayerControllerGameMode(const FP11MessageInfo& MessageInfo);
	UFUNCTION(Client, Unreliable)
	void Client_MessageSendToGameMode(const FP11MessageInfo& MessageInfo);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	void MainMenuInput();
	void ScoreboardInput(const bool bVisible);
	void ChatInput();

public:
	FP11OnMessageSendToGameModeHandle OnMessageSendToGameMode;

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
