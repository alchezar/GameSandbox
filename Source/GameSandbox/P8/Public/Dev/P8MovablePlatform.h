// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "P8MovablePlatform.generated.h"

UCLASS()
class GAMESANDBOX_API AP8MovablePlatform : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	AP8MovablePlatform();
	virtual void Tick(const float DeltaTime) override;
	void SetTriggerActivation(const bool bActive);

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditInstanceOnly, Category = "C++")
	float Speed = 20.f;
	UPROPERTY(EditInstanceOnly, Category = "C++", meta = (MakeEditWidget = "true"))
	FVector TargetLocation = FVector::ZeroVector;

private:
	FVector GlobalStartLocation;
	FVector GlobalTargetLocation;
};
