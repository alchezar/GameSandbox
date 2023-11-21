// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "P12PlayerController.generated.h"

class AP12HUD;
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
	FORCEINLINE bool GetIsIgnoreCameraPitch() const { return bIgnoreCameraPitch; }
	void SetIsIgnoreCameraPitch(const bool bIgnore) { bIgnoreCameraPitch = bIgnore; }
	void ToggleMenuInputMode(const bool bMenu);
	void OnInteractableObjectFound(bool bFound);
	FKey FindFireMappedKey();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	void MoveInput(const FInputActionValue& Value);
	void LookInput(const FInputActionValue& Value);
	void JumpInput();
	void MantleInput();
	void CrouchInput();
	void RunInput(const bool bRun);
	void LadderJumpInput();
	void LadderClimbInput(const FInputActionValue& Value);

	void FireInput(const bool bStart);
	void AimInput(const bool bStart);
	void ReloadInput();
	void EquipItemInput(const bool bNext);
	void EquipThrowableInput();

	void PrimaryMeleeInput();
	void SecondaryMeleeInput();

	void ShowMainMenu();

	void InteractInput();

private:
	void SubsystemDefaultMappingContext() const;
	bool GetCharacterCanProcessInput() const;
	UFUNCTION(Exec)
	void P12Debug_EnableAll();
	UFUNCTION(Exec)
	void P12Debug_DisableAll();

	void CacheHUD();
	void FindInteractMappedKeyName();
	
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
	UPROPERTY(EditAnywhere, Category = "C++")
	UInputAction* MantleAction = nullptr;
	UPROPERTY(EditAnywhere, Category = "C++")
	UInputAction* LadderJumpAction = nullptr;
	UPROPERTY(EditAnywhere, Category = "C++")
	UInputAction* LadderClimbAction = nullptr;
	UPROPERTY(EditAnywhere, Category = "C++")
	UInputAction* FireAction = nullptr;
	UPROPERTY(EditAnywhere, Category = "C++")
	UInputAction* AimAction = nullptr;
	UPROPERTY(EditAnywhere, Category = "C++")
	UInputAction* ReloadAction = nullptr;
	UPROPERTY(EditAnywhere, Category = "C++")
	UInputAction* NextItemAction = nullptr;
	UPROPERTY(EditAnywhere, Category = "C++")
	UInputAction* PreviousItemAction = nullptr;
	UPROPERTY(EditAnywhere, Category = "C++")
	UInputAction* EquipProjectileAction = nullptr;
	UPROPERTY(EditAnywhere, Category = "C++")
	UInputAction* PrimaryMeleeAction = nullptr;
	UPROPERTY(EditAnywhere, Category = "C++")
	UInputAction* SecondaryMeleeAction = nullptr;
	UPROPERTY(EditAnywhere, Category = "C++")
	UInputAction* MainMenuAction = nullptr;
	UPROPERTY(EditAnywhere, Category = "C++")
	UInputAction* InteractAction = nullptr;

private:
	TSoftObjectPtr<AP12BaseCharacter> CachedBaseCharacter;
	bool bIgnoreCameraPitch = false;

	FName InteractKeyName = "NONE";
	TWeakObjectPtr<AP12HUD> CachedHUD;
};
