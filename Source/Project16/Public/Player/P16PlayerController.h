// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "P16PlayerController.generated.h"

class IP16InterfaceEnemy;
class AP16Character;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

UCLASS()
class PROJECT16_API AP16PlayerController : public APlayerController
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
public:
	AP16PlayerController();

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;
	virtual void Tick(const float DeltaSeconds) override;
	virtual void SetupInputComponent() override;

	/* ------------------------------- This -------------------------------- */
protected:
	void MoveInputCallback(const FInputActionValue& InputValue);
	void CursorTrace();

private:
	void AddDefaultMappingContext() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Input")
	TObjectPtr<UInputMappingContext> MappingContext = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Input")
	TObjectPtr<UInputAction> MoveAction = nullptr;

private:
	TScriptInterface<IP16InterfaceEnemy> LastTickEnemy = {};
};
