// Copyright (C) 2023, IKinder

#include "P7/Public/Item/Weapon/P7LightSaber.h"
#include "Components/PointLightComponent.h"

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

void AP7LightSaber::SwitchSaber(const bool bOn)
{
	if (bBeamActive == bOn) return;

	bBeamActive = bOn;
	bOn ? Beam->SetVisibility(bOn) : SaberLight->SetVisibility(bOn);

	FTimerDelegate SwitchDelegate;
	SwitchDelegate.BindUObject(this, &ThisClass::SwitchingBeamSmoothly);
	GetWorld()->GetTimerManager().SetTimer(BeamTimer, SwitchDelegate, GetWorld()->GetDeltaSeconds(), true);
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
