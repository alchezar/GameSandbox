// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "P8GameInstance.generated.h"

UCLASS()
class GAMESANDBOX_API UP8GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UP8GameInstance(const FObjectInitializer& ObjectInitializer);
	virtual void Init() override;
	
	UFUNCTION(Exec)
	void LoadMenu() const;
	UFUNCTION(Exec)
	void Host() const;
	UFUNCTION(Exec)
	void Join(const FString& Address) const;

private:
	TSubclassOf<UUserWidget> MainMenuClass;
};
