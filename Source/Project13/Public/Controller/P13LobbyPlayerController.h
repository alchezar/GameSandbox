// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UI/Menu/P13LobbyWidget.h"
#include "P13LobbyPlayerController.generated.h"

class AP13LobbyGameMode;
class UP13LobbyMenuWidget;

UCLASS()
class PROJECT13_API AP13LobbyPlayerController : public APlayerController
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
public:
	AP13LobbyPlayerController();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

	/* ------------------------------- This -------------------------------- */
public:
	FLinearColor GetOccupiedColor() const;
	UFUNCTION(Server, Reliable)
	void Server_UpdateClientReady();
	void OnHostSelectedMap(const FText& SelectedLevelName) const;
	void UpdateSelectedMapName(const FText& SelectedLevelName) const;
	void OnLogin();
	UFUNCTION(Server, Reliable)
	void Server_OnPlayerColorSelected(const FLinearColor SelectedColor);
	UFUNCTION(Client, Reliable)
	void Client_UpdateSelectedColorOccupation(const FLinearColor SelectedColor, const FLinearColor ReleasedColor, const AP13LobbyPlayerController* Occupier) const;
	void OccupyTargetPoint(ATargetPoint* InTargetPoint);
	ATargetPoint* ReleaseTargetPoint();

protected:
	bool ShowLobbyMenu();
	void SetMenuInputMode();
	void ListenToGameMode();

	/* ----------------------------- Variables ----------------------------- */
protected:
	UPROPERTY(EditAnywhere, Category = "C++")
	TSubclassOf<UP13LobbyMenuWidget> LobbyMenuWidgetClass;

	UPROPERTY(Replicated)
	FName CurrentLevelByServer;

private:
	UPROPERTY()
	UP13LobbyMenuWidget* LobbyMenuWidget = nullptr;
	TSoftObjectPtr<AP13LobbyGameMode> CachedLobbyGameMode;
	TWeakObjectPtr<ATargetPoint> CachedTargetPoint;
};
