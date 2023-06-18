// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ARPlayerController.generated.h"

class UInputAction;
class UInputMappingContext;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAROnPawnChangedSignature, APawn*, NewPawn);

UCLASS()
class GAMESANDBOX_API AARPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AARPlayerController();
	virtual void Tick(float DeltaTime) override;
	virtual void SetPawn(APawn* InPawn) override;
	
	void TogglePauseMenu();

protected:
	virtual void BeginPlay() override;
	virtual void BeginPlayingState() override;
	virtual void SetupInputComponent() override;

private:
	void AddWidget();
	void AddDefaultMappingContext();

public:
	FAROnPawnChangedSignature OnPawnChanged;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Interface")
	TSubclassOf<UUserWidget> HUDClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Interface")
	TSubclassOf<UUserWidget> PauseMenuClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ | Input")
	UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ | Input")
	UInputAction* PauseAction;
	
private:
	UPROPERTY()
	UUserWidget* HUDWidget;
	UPROPERTY()
	UUserWidget* PauseMenuInstance;
	bool bPause = false;
	
};
