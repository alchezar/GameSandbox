// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LS_LightSaber.generated.h"

class UPointLightComponent;
class UNiagaraSystem;
class UNiagaraComponent;

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
	void EnableRibbon();
	void DisableRibbon();

	bool GetIsTurnedOn() const;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Effect")
	UNiagaraSystem* RibbonEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Effect")
	FName RibbonColorName = "RibbonColor";
	
private:
	UPROPERTY()
	UMaterialInstanceDynamic* SaberColorMaterial;
	UPROPERTY()
	UNiagaraComponent* RibbonComponent;
	bool bTurnedOn = true;
	FTimerHandle BeamLightTimer;
	float CurrentZScale = 1.f;
};
