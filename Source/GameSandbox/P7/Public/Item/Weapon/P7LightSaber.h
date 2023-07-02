// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "P7/Public/Item/Weapon/P7Weapon.h"
#include "P7LightSaber.generated.h"

class UNiagaraComponent;
class UPointLightComponent;
class UAudioComponent;

UCLASS()
class GAMESANDBOX_API AP7LightSaber : public AP7Weapon
{
	GENERATED_BODY()

public:
	AP7LightSaber();
	virtual void Tick(float DeltaTime) override;
	virtual void SwitchWeapon(const bool bOn) override;
	virtual void SwitchWeaponHard(const bool bOn) override;
virtual void OnWeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
virtual void OnWeaponEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	
protected:
	virtual void BeginPlay() override;
	virtual void SwitchRibbon(const bool bOn) override;
	virtual void SplashEffect(const FHitResult& HitResult) override;

private:
	void SwitchingBeamSmoothly();
	void SetSaberColor(const FLinearColor NewColor);
	void TouchTrace();
	void PlasmaDecal(const FHitResult& HitResult);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	UStaticMeshComponent* Beam;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	UPointLightComponent* SaberLight;
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	USceneComponent* TraceStart;
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	USceneComponent* TraceEnd;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Light")
	FName ColorParameterName = "Color";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Light")
	FLinearColor Color = FLinearColor(0.5f, 0.f, 1.f, 1.f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Light")
	float BeamGlowIntensity = 1500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Light")
	float TurningStep = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Effect")
	FP7DecalEffect Plasma;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Effect")
	float SpawnStep = 1.f;

private:
	bool bBeamActive = false;
	FTimerHandle BeamTimer;
	float CurrentBeamScale = 1.f;
	UPROPERTY()
	UMaterialInstanceDynamic* SaberColorMaterial;
	UPROPERTY()
	UNiagaraComponent* RibbonComponent;
	UPROPERTY()
	UAudioComponent* HummingSound;
	bool bBladeTouch = false;
	UPROPERTY()
	TArray<AActor*> OverlappedActors;
	TArray<FP7PlasmaSpawn> PlasmaContacts;
	TArray<FVector> AllPlasmaDecalLocations;
};
