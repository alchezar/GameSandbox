// Copyright (C) 2023, IKinder

#include "LS_LightSaber.h"
#include "DrawDebugHelpers.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Components/DecalComponent.h"
#include "Components/PointLightComponent.h"
#include "Kismet/GameplayStatics.h"
#include "P5/Content/LS_Slicable.h"
#include "P5/Player/LS_BaseCharacter.h"

ALS_LightSaber::ALS_LightSaber()
{
	PrimaryActorTick.bCanEverTick = true;
	SetupMesh();
}

void ALS_LightSaber::BeginPlay()
{
	Super::BeginPlay();
	// SetSaberColor(Color);
	check(Blade);
	Blade->OnComponentHit.AddDynamic(this, &ThisClass::OnBladeHit);
	Blade->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBladeBeginOverlap);
	Blade->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnBladeEndOverlap);
}

void ALS_LightSaber::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	LineTrace();
}

void ALS_LightSaber::SetupMesh()
{
	Saber = CreateDefaultSubobject<UStaticMeshComponent>("Saber");
	SetRootComponent(Saber);
	Saber->SetCollisionProfileName("NoCollision");

	Beam = CreateDefaultSubobject<UStaticMeshComponent>("Beam");
	Beam->SetupAttachment(GetRootComponent());
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

	Blade = CreateDefaultSubobject<UCapsuleComponent>("CutTrigger");
	Blade->SetupAttachment(GetRootComponent());
	Blade->SetCollisionProfileName("OverlapAll");
	Blade->SetRelativeLocation(FVector(0.f, 0.f, 65.f));
	Blade->SetCapsuleRadius(5.f);
	Blade->SetNotifyRigidBodyCollision(true); // Simulation Generates Hit Events
}

void ALS_LightSaber::SetSaberColor(const FLinearColor NewColor)
{
	if (!Beam || !SaberLight) return;

	Color = NewColor;
	SaberColorMaterial = Beam->CreateAndSetMaterialInstanceDynamic(0);
	if (!SaberColorMaterial) return;

	SaberColorMaterial->SetVectorParameterValue(ColorParameterName, Color);
	SaberLight->SetLightColor(Color);
}

void ALS_LightSaber::TurnBeamOn()
{
	if (bTurnedOn) return;

	bTurnedOn = true;
	Beam->SetVisibility(true);

	FTimerDelegate TurnOnDelegate;
	TurnOnDelegate.BindUObject(this, &ThisClass::TurnBeamGradual, true);
	GetWorldTimerManager().SetTimer(BeamLightTimer, TurnOnDelegate, GetWorld()->GetDeltaSeconds(), true);
}

void ALS_LightSaber::TurnBeamOff()
{
	if (!bTurnedOn) return;

	bTurnedOn = false;
	SaberLight->SetVisibility(false);

	FTimerDelegate TurnOffDelegate;
	TurnOffDelegate.BindUObject(this, &ThisClass::TurnBeamGradual, false);
	GetWorldTimerManager().SetTimer(BeamLightTimer, TurnOffDelegate, GetWorld()->GetDeltaSeconds(), true);
}

void ALS_LightSaber::TurnBeamGradual(bool bEnabling)
{
	const float TargetZScale = bEnabling ? 1.f : 0.f;
	const float ScaleDirection = bEnabling ? 1.f : -1.f;
	const bool bDone = Beam->GetRelativeScale3D().Z == TargetZScale;

	CurrentZScale = FMath::Clamp((CurrentZScale + TurningStep * ScaleDirection), 0.f, 1.f);
	Beam->SetRelativeScale3D(FVector(1.f, 1.f, CurrentZScale));
	SaberLight->SetIntensity(FMath::Clamp(SaberLight->Intensity + ScaleDirection * TurningStep * BeamGlowIntensity, 0.f, BeamGlowIntensity));

	if (bEnabling && bDone)
	{
		SaberLight->SetVisibility(true);
		GetWorldTimerManager().ClearTimer(BeamLightTimer);
		return;
	}
	if (!bEnabling && bDone)
	{
		Beam->SetVisibility(false);
		GetWorldTimerManager().ClearTimer(BeamLightTimer);
		return;
	}
}

void ALS_LightSaber::EnableRibbon()
{
	if (RibbonComponent)
	{
		RibbonComponent->Activate();
		return;
	}
	/* Create niagara on the first try */
	RibbonComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
		Ribbon.Effect, GetRootComponent(), NAME_None, FVector(0.f, 0.f, 61.f), FRotator::ZeroRotator, EAttachLocation::SnapToTarget, false);
	if (!RibbonComponent) return;

	RibbonComponent->SetVariableLinearColor(Ribbon.ColorName, Color);
}

void ALS_LightSaber::DisableRibbon()
{
	if (!RibbonComponent) return;

	RibbonComponent->Deactivate();
}

bool ALS_LightSaber::GetIsTurnedOn() const
{
	return bTurnedOn;
}

void ALS_LightSaber::SetSaberOwner(AActor* CurrentOwner)
{
	OwnerCharacter = Cast<ALS_BaseCharacter>(CurrentOwner);
}

void ALS_LightSaber::LineTrace()
{
	if (!bTurnedOn) return;

	const FVector Start = Beam->GetSocketLocation(BaseSocketName);
	const FVector End = Beam->GetSocketLocation(TipSocketName);
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility);
	if (!HitResult.bBlockingHit || HitResult.GetActor() == OwnerCharacter)
	{
		if (!PlasmaContacts.IsEmpty()) PlasmaContacts.Empty();
		return;
	}
	SplashEffect(HitResult);
	GenerateMoreDecalPoints(HitResult);
	PlasmaDecal(HitResult);

	if (OwnerCharacter && OwnerCharacter->GetIsAttacking())
	{
		SliceProcedural(HitResult);
	}
}

void ALS_LightSaber::SplashEffect(const FHitResult& HitResult)
{
	UNiagaraComponent* SplashComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(), Contact.Effect, HitResult.Location, HitResult.Normal.Rotation());
	if (!SplashComponent) return;

	SplashComponent->SetVariableLinearColor(Contact.ColorName, Color);
}

void ALS_LightSaber::GenerateMoreDecalPoints(const FHitResult& HitResult)
{
	PlasmaContacts.Add(FLSPlasmaSpawn(HitResult));
	if (PlasmaContacts.Num() < 2) return;

	const int32 Distance = (PlasmaContacts[1].Location - PlasmaContacts[0].Location).Size();
	for (int i = 0; i < Distance; ++i)
	{
		AllPlasmaDecalLocations.Add(
			FMath::VInterpTo(PlasmaContacts[0].Location, PlasmaContacts[1].Location, i, 1.f / Distance));
	}
	PlasmaContacts.RemoveAt(0);
}

void ALS_LightSaber::PlasmaDecal(const FHitResult& HitResult)
{
	for (const auto Location : AllPlasmaDecalLocations)
	{
		UDecalComponent* PlasmaComponent = UGameplayStatics::SpawnDecalAtLocation(
			GetWorld(), Plasma.Material, FVector(Plasma.Size), Location, HitResult.ImpactNormal.Rotation());
		if (!PlasmaComponent) return;

		PlasmaComponent->SetFadeOut(5.f, 1.f, false);
		UMaterialInstanceDynamic* DynamicPlasma = PlasmaComponent->CreateDynamicMaterialInstance();
		if (!DynamicPlasma) return;

		DynamicPlasma->SetVectorParameterValue(Plasma.ColorName, Color);
	}
	AllPlasmaDecalLocations.Empty();
}

void ALS_LightSaber::SliceProcedural(const FHitResult& HitResult)
{
	ALS_Slicable* SlicableActor = Cast<ALS_Slicable>(HitResult.GetActor());
	if (!SlicableActor) return;

	const FVector CutNormal = Beam->GetRightVector();
	SlicableActor->Slice(HitResult.Location, CutNormal, SaberColorMaterial);
}

void ALS_LightSaber::OnBladeHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("hit")));
}

void ALS_LightSaber::OnBladeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == OwnerCharacter) return;

	// GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("start")));
}

void ALS_LightSaber::OnBladeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == OwnerCharacter) return;

	// GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("end")));
}
