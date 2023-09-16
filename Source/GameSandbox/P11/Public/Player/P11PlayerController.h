// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "P11PlayerController.generated.h"

class AP11Character;
enum class EP11PlayerSide : uint8;
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
	void Server_ChangeCharSide(EP11PlayerSide NewSide);
	void ChangeCharSide(EP11PlayerSide NewSide);

	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	void MainMenuInput();

protected:
	UPROPERTY(EditAnywhere, Category = "C++ | Input")
	UInputMappingContext* DefaultContext;
	UPROPERTY(EditAnywhere, Category = "C++ | Input")
	UInputAction* MainMenuAction;

private:
	bool bMenuVisibility = false;
};
