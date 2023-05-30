// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LS_LightSaber.generated.h"

class UPointLightComponent;

UCLASS()
class GAMESANDBOX_API ALS_LightSaber : public AActor
{
	GENERATED_BODY()

public:
	ALS_LightSaber();
	virtual void Tick(float DeltaTime) override;

	void TurnBeamOn();
	void TurnBeamOff();
	void SetSaberColor(FLinearColor NewColor);

protected:
	virtual void BeginPlay() override;

private:
	void SetupMesh();
	void TurnBeamGradual(bool bEnabling);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | Mesh")
	UStaticMeshComponent* Saber;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | Mesh")
	UStaticMeshComponent* Beam;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | Mesh")
	UPointLightComponent* SaberLight;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Light")
	FName ColorParameterName = "Color";	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Light")
	FLinearColor Color = FLinearColor(0.5f, 0.f, 1.f, 1.f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Light")
	float BeamGlowIntensity = 1500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Light")
	float TurningStep = 0.1f;
	
private:
	UPROPERTY()
	UMaterialInstanceDynamic* SaberColorMaterial;
	bool bTurnedOn = true;
	FTimerHandle BeamLightTimer;
	float CurrentZScale = 1.f;
	
};
