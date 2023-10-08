// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "P12PlayerController.generated.h"

class AP12BaseCharacter;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;

UCLASS()
class GAMESANDBOX_API AP12PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AP12PlayerController();
	virtual void SetPawn(APawn* InPawn) override;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	void MoveInput(const FInputActionValue& Value);
	void LookInput(const FInputActionValue& Value);
	void JumpInput();
	void CrouchInput();
	void RunInput(const bool bRun);

private:
	void SubsystemDefaultMappingContext() const;

protected:
	UPROPERTY(EditAnywhere, Category = "C++")
	UInputMappingContext* DefaultContext = nullptr;
	UPROPERTY(EditAnywhere, Category = "C++")
	UInputAction* MoveAction = nullptr;
	UPROPERTY(EditAnywhere, Category = "C++")
	UInputAction* LookAction = nullptr;
	UPROPERTY(EditAnywhere, Category = "C++")
	UInputAction* JumpAction = nullptr;
	UPROPERTY(EditAnywhere, Category = "C++")
	UInputAction* CrouchAction = nullptr;
	UPROPERTY(EditAnywhere, Category = "C++")
	UInputAction* RunAction = nullptr;
	
private:
	TSoftObjectPtr<AP12BaseCharacter> CachedBaseCharacter;
};
