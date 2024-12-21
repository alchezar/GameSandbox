// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ARPlayerState.generated.h"

class UARSaveGame;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAROnCreditsChangedSignature, APlayerState*, PlayerState, int32, Credits, int32, Delta);

UCLASS()
class PROJECT06_API AARPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AARPlayerState();
	virtual void Tick(float DeltaTime) override;

	int32 GetCredits() const;
	void AddCredits(const int32 Delta);
	bool RemoveCredits(const int32 Delta);

	void SavePlayerState(UARSaveGame* SaveObject);
	void LoadPlayerState(const UARSaveGame* SaveObject);

	/* Multiplayer */
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnRep_Credits(const int32 OldCredits);

public:
	FAROnCreditsChangedSignature OnCreditsChanged;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, ReplicatedUsing = OnRep_Credits, Category = "C++ | Credits")
	int32 Credits;
};
