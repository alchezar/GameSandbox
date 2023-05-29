// Copyright (C) 2023, IKinder

#include "LS_LightSaber.h"
#include "Components/PointLightComponent.h"

ALS_LightSaber::ALS_LightSaber()
{
	PrimaryActorTick.bCanEverTick = true;
	SetupMesh();
}

void ALS_LightSaber::BeginPlay()
{
	Super::BeginPlay();
	// SetSaberColor(Color);	
}

void ALS_LightSaber::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALS_LightSaber::SetupMesh()
{	
	Saber = CreateDefaultSubobject<UStaticMeshComponent>("Saber");
	SetRootComponent(Saber);
	Saber->SetCollisionProfileName("NoCollision");

	Beam = CreateDefaultSubobject<UStaticMeshComponent>("Beam");
	Beam->SetupAttachment(GetRootComponent());
	Beam->SetCollisionProfileName("NoCollision");

	SaberLight = CreateDefaultSubobject<UPointLightComponent>("SaberLight");
	SaberLight->SetupAttachment(Beam);
	SaberLight->SetRelativeLocation(FVector(0.f, 0.f, 65.f));
	SaberLight->SetAttenuationRadius(100.f);
	SaberLight->SetIntensity(1500.f);
	SaberLight->SetSourceRadius(5.f);
	SaberLight->SetSourceLength(80.f);
	SaberLight->bCastShadowsFromCinematicObjectsOnly = true;
}

void ALS_LightSaber::SetSaberColor(const FLinearColor NewColor)
{
	if (!Beam || !SaberLight) return;
	
	SaberColorMaterial = Beam->CreateAndSetMaterialInstanceDynamic(0);
	if (!SaberColorMaterial) return;

	SaberColorMaterial->SetVectorParameterValue(ColorParameterName, NewColor);
	SaberLight->SetLightColor(NewColor);
}
