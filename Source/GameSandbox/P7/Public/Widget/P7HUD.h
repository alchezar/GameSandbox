// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "P7HUD.generated.h"

class UP7PlayerOverlay;

UCLASS()
class GAMESANDBOX_API AP7HUD : public AHUD
{
	GENERATED_BODY()

public:
	AP7HUD();
	FORCEINLINE UP7PlayerOverlay* GetPlayerOverlay() const { return PlayerOverlayWidget; }; 
	
protected:
	virtual void BeginPlay() override;

private:
	void SetCharacterInWidget();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++")
	TSubclassOf<UP7PlayerOverlay> PlayerOverlayClass;

private:
	UPROPERTY()
	UP7PlayerOverlay* PlayerOverlayWidget;
};
