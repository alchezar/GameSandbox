// Copyright (C) 2023, IKinder

#include "Weapon/Component/STU_WeaponFXComponent.h"

#include "NiagaraFunctionLibrary.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

USTU_WeaponFXComponent::USTU_WeaponFXComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USTU_WeaponFXComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USTU_WeaponFXComponent::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USTU_WeaponFXComponent::PlayImpactFX(const FHitResult& Hit)
{
	FImpactData ImpactData = DefaultImpactData;
	if (Hit.PhysMaterial.IsValid())
	{
		const UPhysicalMaterial* PhysicalMaterial = Hit.PhysMaterial.Get();
		if (ImpactDataMap.Contains(PhysicalMaterial))
		{
			ImpactData = ImpactDataMap[PhysicalMaterial];
		}
	}

	// Niagara
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		ImpactData.NiagaraEffect,
		Hit.ImpactPoint,
		Hit.ImpactNormal.Rotation());

	// Decal
	UDecalComponent* DecalComponent;
	if (Hit.BoneName != FName("None"))
	{
		DecalComponent = UGameplayStatics::SpawnDecalAttached(
			ImpactData.DecalData.Material,
			ImpactData.DecalData.Size,
			Hit.GetComponent(),
			Hit.BoneName,
			Hit.ImpactPoint,
			Hit.ImpactNormal.Rotation(),
			EAttachLocation::KeepWorldPosition,
			0.f);
	}
	else
	{
		DecalComponent = UGameplayStatics::SpawnDecalAtLocation(
			GetWorld(),
			ImpactData.DecalData.Material,
			ImpactData.DecalData.Size,
			Hit.ImpactPoint,
			Hit.ImpactNormal.Rotation(),
			ImpactData.DecalData.Lifetime + ImpactData.DecalData.FadeOutTime);
	}

	if (DecalComponent)
	{
		DecalComponent->SetFadeOut(ImpactData.DecalData.Lifetime, ImpactData.DecalData.FadeOutTime, false);
	}

	// Sound
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactData.Sound, Hit.ImpactPoint);
}
