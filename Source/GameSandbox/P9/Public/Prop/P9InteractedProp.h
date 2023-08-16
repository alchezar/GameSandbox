// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P9/Public/Interface/P9Interaction.h"
#include "P9InteractedProp.generated.h"

UCLASS()
class GAMESANDBOX_API AP9InteractedProp : public AActor, public IP9Interaction
{
	GENERATED_BODY()

public:
	AP9InteractedProp();
	virtual void Tick(float DeltaTime) override;
	virtual void Interact(ACharacter* Causer) override;

protected:
	virtual void BeginPlay() override;

private:
	void ScaleMesh();

protected:
	UPROPERTY(EditAnywhere, Category = "C++ | Component")
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditAnywhere, Category = "C++ | Interact")
	bool bCanScale = true;

private:
	bool bBig = false;
	FTimerHandle ScaleTimer;
};
