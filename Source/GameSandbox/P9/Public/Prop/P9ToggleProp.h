// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "P9InteractedProp.h"
#include "P9ToggleProp.generated.h"

UCLASS()
class GAMESANDBOX_API AP9ToggleProp : public AP9InteractedProp
{
	GENERATED_BODY()

public:
	AP9ToggleProp();
	virtual void Tick(float DeltaTime) override;
	virtual void Interact(ACharacter* Causer) override;

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, Category = "C++ | Tag")
	TArray<FName> ToggleTags;

private:
	TArray<IP9Interaction*> Interfaces;
};
