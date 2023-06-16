// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ARPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAROnPawnChangedSignature, APawn*, NewPawn);

UCLASS()
class GAMESANDBOX_API AARPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AARPlayerController();
	virtual void Tick(float DeltaTime) override;
	virtual void SetPawn(APawn* InPawn) override;
	
protected:
	virtual void BeginPlay() override;

private:
	void AddWidget();

public:
	FAROnPawnChangedSignature OnPawnChanged;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Interface")
	TSubclassOf<UUserWidget> HUDClass;

private:
	UPROPERTY()
	UUserWidget* HUDWidget;
	
};
