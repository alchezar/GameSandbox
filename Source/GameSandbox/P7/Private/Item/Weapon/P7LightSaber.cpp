// Copyright (C) 2023, IKinder

#include "P7/Public/Item/Weapon/P7LightSaber.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Components/PointLightComponent.h"
#include "Kismet/GameplayStatics.h"

AP7LightSaber::AP7LightSaber()
{
	PrimaryActorTick.bCanEverTick = true;

	Beam = CreateDefaultSubobject<UStaticMeshComponent>("BeamMesh");
	Beam->SetupAttachment(ItemMesh);
	Beam->SetCollisionProfileName("NoCollision");
	Beam->bCastStaticShadow = false;
	Beam->bCastDynamicShadow = false;

	SaberLight = CreateDefaultSubobject<UPointLightComponent>("SaberLight");
	SaberLight->SetupAttachment(Beam);
	SaberLight->SetRelativeLocation(FVector(0.f, 0.f, 60.f));
	SaberLight->SetAttenuationRadius(100.f);
	SaberLight->SetIntensity(BeamGlowIntensity);
	SaberLight->SetSourceRadius(5.f);
	SaberLight->SetSourceLength(80.f);
	SaberLight->bCastShadowsFromCinematicObjectsOnly = true;
}

void AP7LightSaber::BeginPlay()
{
	Super::BeginPlay();
	SetSaberColor(Color);
}

void AP7LightSaber::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP7LightSaber::SwitchWeapon(const bool bOn)
{
	// if (bBeamActive == bOn) return;
	bBeamActive = bOn;	
	if (bOn)
	{
		Beam->SetVisibility(bOn);
		if (!WeaponSound.Arm || !WeaponSound.Humming) return;
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), WeaponSound.Arm, GetActorLocation());
		HummingSound ? HummingSound->Play() : HummingSound = UGameplayStatics::SpawnSoundAttached(WeaponSound.Humming, RootComponent);
	}
	else
	{
		SaberLight->SetVisibility(bOn);
		if (!HummingSound || !WeaponSound.Disarm) return;
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), WeaponSound.Disarm, GetActorLocation());
		HummingSound->Stop();
	}	

	FTimerDelegate SwitchDelegate;
	SwitchDelegate.BindUObject(this, &ThisClass::SwitchingBeamSmoothly);
	GetWorld()->GetTimerManager().SetTimer(BeamTimer, SwitchDelegate, GetWorld()->GetDeltaSeconds(), true);
}

void AP7LightSaber::SwitchWeaponHard(const bool bOn)
{
	bBeamActive = bOn;
	SaberLight->SetVisibility(bOn);
	SaberLight->SetIntensity(bOn ? BeamGlowIntensity : 0.f);
	Beam->SetVisibility(bOn);
	Beam->SetRelativeScale3D(FVector(bOn ? 1.f : 0.f));
	SwitchRibbon(false);
}

void AP7LightSaber::SwitchingBeamSmoothly()
{
	const float TargetScale = bBeamActive ? 1.f : 0.f;
	const float ScaleDirection = bBeamActive ? 1.f : -1.f;

	CurrentBeamScale = FMath::Clamp((CurrentBeamScale + TurningStep * ScaleDirection), 0.f, 1.f);
	Beam->SetRelativeScale3D(FVector(1.f, 1.f, CurrentBeamScale));
	SaberLight->SetIntensity(FMath::Clamp(SaberLight->Intensity + ScaleDirection * TurningStep * BeamGlowIntensity, 0.f, BeamGlowIntensity));

	if (Beam->GetRelativeScale3D().Z != TargetScale) return;
	bBeamActive ? SaberLight->SetVisibility(bBeamActive) : Beam->SetVisibility(bBeamActive);
	GetWorldTimerManager().ClearTimer(BeamTimer);
}

void AP7LightSaber::SetSaberColor(const FLinearColor NewColor)
{
	if (!Beam || !SaberLight) return;

	Color = NewColor;
	SaberColorMaterial = Beam->CreateAndSetMaterialInstanceDynamic(0);
	if (!SaberColorMaterial) return;
	
	SaberColorMaterial->SetVectorParameterValue(ColorParameterName, Color);
	SaberLight->SetLightColor(Color);
}

void AP7LightSaber::SwitchRibbon(const bool bOn)
{
	if (RibbonComponent)
	{
		if (!bOn)
		{
			RibbonComponent->Deactivate();
			return;
		}	
		RibbonComponent->Activate();
		return;
	}	
	/* Create niagara on the first try */
	RibbonComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
		Ribbon.Effect, GetRootComponent(), NAME_None, FVector(0.f, 0.f, 61.f), FRotator::ZeroRotator, EAttachLocation::SnapToTarget, false);
	if (!RibbonComponent) return;

	RibbonComponent->SetVariableLinearColor(Ribbon.ColorName, Color);
	
}

void AP7LightSaber::SplashEffect(const FHitResult& HitResult)
{	
	if (UNiagaraComponent* SplashNiagara = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(), Contact.Effect, HitResult.ImpactPoint, HitResult.ImpactNormal.Rotation()))
	{
		SplashNiagara->SetVariableLinearColor(Contact.ColorName, Color);
	}
}
