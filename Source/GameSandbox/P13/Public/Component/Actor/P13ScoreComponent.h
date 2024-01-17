// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "P13ScoreComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMESANDBOX_API UP13ScoreComponent : public UActorComponent
{
	GENERATED_BODY()

	/* ------------------------------- Super ------------------------------- */
public:
	UP13ScoreComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* ------------------------------- This -------------------------------- */
protected:
	void OnOwnerDeadHandle(AController* Causer);

private:
	void ConnectToHealth();

	/* ----------------------------- Variables ----------------------------- */
protected:
	UPROPERTY(EditAnywhere, Category = "C++")
	int32 Score = 10;
};
