// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "P7/Public/Item/Weapon/P7Weapon.h"
#include "P7LightSaber.generated.h"

class UPointLightComponent;

UCLASS()
class GAMESANDBOX_API AP7LightSaber : public AP7Weapon
{
	GENERATED_BODY()

public:
	AP7LightSaber();
	virtual void Tick(float DeltaTime) override;
	void SwitchSaber(const bool bOn);

protected:
	virtual void BeginPlay() override;

private:
	void SwitchingBeamSmoothly();
	void SetSaberColor(const FLinearColor NewColor);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	UStaticMeshComponent* Beam;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	UPointLightComponent* SaberLight;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Light")
	FName ColorParameterName = "Color";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Light")
	FLinearColor Color = FLinearColor(0.5f, 0.f, 1.f, 1.f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Light")
	float BeamGlowIntensity = 1500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Light")
	float TurningStep = 0.1f;

private:
	bool bBeamActive = true;
	FTimerHandle BeamTimer;
	float CurrentBeamScale = 1.f;
	UPROPERTY()
	UMaterialInstanceDynamic* SaberColorMaterial;
};
