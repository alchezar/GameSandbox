// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P16SpawnPoint.h"
#include "P16SpawnVolume.generated.h"

class UBoxComponent;

UCLASS()
class PROJECT16_API AP16SpawnVolume : public AP16SpawnPoint
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
public:
	AP16SpawnVolume();

protected:
	virtual void BeginPlay() override;

	/// ------------------------------------------------------------------------
	/// @name Super
	/// ------------------------------------------------------------------------
public:
	virtual void Spawn() override;

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++")
	TObjectPtr<UBoxComponent> SpawnVolume = nullptr;
};
