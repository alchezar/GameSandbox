// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ARPlayerController.generated.h"

UCLASS()
class GAMESANDBOX_API AARPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AARPlayerController();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	void AddWidget();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Interface")
	TSubclassOf<UUserWidget> HUDClass;

private:
	UPROPERTY()
	UUserWidget* HUDWidget;
	
};
