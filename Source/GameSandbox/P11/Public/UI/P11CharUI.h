// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"

#include "P11CharUI.generated.h"

enum class EP11PlayerSide : uint8;
class USpotLightComponent;

UCLASS()
class GAMESANDBOX_API AP11CharUI : public AActor
{
	GENERATED_BODY()

public:
	AP11CharUI();
	virtual void Tick(float DeltaTime) override;
	void ChoseSide(const EP11PlayerSide NewSide) const;

protected:
	virtual void BeginPlay() override;

private:
	void FindDefaultReferences() const;

protected:
	UPROPERTY(EditAnywhere, Category = "C++")
	USkeletalMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, Category = "C++")
	USpotLightComponent* Light;
	UPROPERTY(EditAnywhere, Category = "C++")
	USceneCaptureComponent2D* Capture;
};
