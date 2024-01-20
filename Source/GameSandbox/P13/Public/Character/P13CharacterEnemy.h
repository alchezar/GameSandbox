// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "P13CharacterBase.h"
#include "P13CharacterEnemy.generated.h"

class UP13ScoreComponent;

UCLASS()
class GAMESANDBOX_API AP13CharacterEnemy : public AP13CharacterBase
{
	GENERATED_BODY()

	/* ------------------------------- Super ------------------------------- */
public:
	AP13CharacterEnemy();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
	/* ------------------------------- This -------------------------------- */
private:
	void CreateEnemyComponents();
	void MoveToPlayer() const;

	/* ----------------------------- Variables ----------------------------- */
protected:
	UPROPERTY(EditAnywhere, Category = "C++")
	UP13ScoreComponent* ScoreComp;

	UPROPERTY(EditAnywhere, Category = "C++")
	float AcceptanceRadius = 200.f;
};
