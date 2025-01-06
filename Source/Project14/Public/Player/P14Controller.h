// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "P14Controller.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class PROJECT14_API AP14Controller : public APlayerController
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
public:
	AP14Controller();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	/* ------------------------------- This -------------------------------- */
public:
	void TogglePause();

	/* ------------------------------ Fields ------------------------------- */
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++")
	TObjectPtr<UInputMappingContext> InputContext = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++")
	TObjectPtr<UInputAction> MenuAction = nullptr;
};
