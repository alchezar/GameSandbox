// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P16MagicCircle.generated.h"

UCLASS()
class PROJECT16_API AP16MagicCircle : public AActor
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
public:
	AP16MagicCircle();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(const float DeltaTime) override;

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
	void SetDecalMaterial(UMaterialInterface* DecalMaterial) const;

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++")
	TObjectPtr<UDecalComponent> MagicCircleDecal = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++")
	float RotationSpeed = 10.f;
};
