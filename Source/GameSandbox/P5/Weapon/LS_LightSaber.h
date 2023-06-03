// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LS_LightSaber.generated.h"

class ALS_BaseCharacter;
class UPointLightComponent;
class UNiagaraSystem;
class UNiagaraComponent;
class UMaterialInterface;
class UCapsuleComponent;

USTRUCT(BlueprintType)
struct FLSNiagaraEffect
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Effect")
	UNiagaraSystem* Effect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Effect")
	FName ColorName = "RibbonColor";	
};

USTRUCT(BlueprintType)
struct FLSDecalEffect
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Effect")
	UMaterialInterface* Material;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Effect")
	float Size = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Effect")
	FName ColorName = "Color";
};

USTRUCT(BlueprintType)
struct FLSPlasmaSpawn
{
	GENERATED_BODY()

	FVector Location;
	FRotator Rotation;

	FLSPlasmaSpawn()
	{
		Location = FVector::ZeroVector;
		Rotation = FRotator::ZeroRotator;
	}

	explicit FLSPlasmaSpawn(const FHitResult& HitResult)
	{
		Location = HitResult.ImpactPoint;
		Rotation = HitResult.ImpactNormal.Rotation();
	}
};

UCLASS()
class GAMESANDBOX_API ALS_LightSaber : public AActor
{
	GENERATED_BODY()

public:
	ALS_LightSaber();
	virtual void Tick(float DeltaTime) override;
	
	bool GetIsTurnedOn() const;
	void SetSaberOwner(AActor* CurrentOwner);

	void TurnBeamOn();
	void TurnBeamOff();
	void SetSaberColor(FLinearColor NewColor);	
	void EnableRibbon();
	void DisableRibbon();
	
protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnBladeHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
	void OnBladeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnBladeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
private:
	void SetupMesh();
	void TurnBeamGradual(bool bEnabling);
	void LineTrace();
	void SplashEffect(const FHitResult& HitResult);
	void PlasmaDecal(const FHitResult& HitResult);
	void GenerateMoreDecalPoints(const FHitResult& HitResult);
	void SliceProcedural(const FHitResult& HitResult);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | Mesh")
	UStaticMeshComponent* Saber;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | Mesh")
	UStaticMeshComponent* Beam;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | Mesh")
	UPointLightComponent* SaberLight;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | Mesh")
	UCapsuleComponent* Blade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Light")
	FName ColorParameterName = "Color";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Light")
	FLinearColor Color = FLinearColor(0.5f, 0.f, 1.f, 1.f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Light")
	float BeamGlowIntensity = 1500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Light")
	float TurningStep = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Effect")
	FLSNiagaraEffect Ribbon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Effect")
	FLSNiagaraEffect Contact;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Effect")
	FLSDecalEffect Plasma;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Trace")
	FName BaseSocketName = "BaseSocket";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Trace")
	FName TipSocketName = "TipSocket";
	
private:
	UPROPERTY()
	UMaterialInstanceDynamic* SaberColorMaterial;
	UPROPERTY()
	UNiagaraComponent* RibbonComponent;
	UPROPERTY()
	ALS_BaseCharacter* OwnerCharacter;
	bool bTurnedOn = true;
	FTimerHandle BeamLightTimer;
	FTimerHandle SliceTimer;
	float CurrentZScale = 1.f;
	TArray<FLSPlasmaSpawn> PlasmaContacts;
	TArray<FVector> AllPlasmaDecalLocations;
};
