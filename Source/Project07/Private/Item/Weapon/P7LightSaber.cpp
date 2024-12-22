// Copyright (C) 2023, IKinder

#include "Item/Weapon/P7LightSaber.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "Components/PointLightComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/P7Enemy.h"

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

	TraceStart = CreateDefaultSubobject<USceneComponent>("TraceStartPoint");
	TraceStart->SetupAttachment(RootComponent);
	TraceStart->SetRelativeLocation(FVector(0.f, 0.f, 20.f));

	TraceEnd = CreateDefaultSubobject<USceneComponent>("TraceEndPoint");
	TraceEnd->SetupAttachment(RootComponent);
	TraceEnd->SetRelativeLocation(FVector(0.f, 0.f, 101.f));
}

void AP7LightSaber::BeginPlay()
{
	Super::BeginPlay();
	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnWeaponBeginOverlap);
	WeaponBox->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnWeaponEndOverlap);
	SetSaberColor(Color);
}

void AP7LightSaber::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
	TouchTrace();
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
		if (HummingSound)
		{
			HummingSound->Play();
		}
		else
		{
			HummingSound = UGameplayStatics::SpawnSoundAttached(WeaponSound.Humming, RootComponent);
		}
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
	SwitchRibbon(bOn);
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

void AP7LightSaber::OnWeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bBladeOverlapped = true;
	OverlappedActors.AddUnique(OtherActor);
}

void AP7LightSaber::OnWeaponEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OverlappedActors.RemoveSingle(OtherActor);
	if (OverlappedActors.IsEmpty())
	{
		bBladeOverlapped = false;
	}
	PlasmaContacts.Empty();
}

void AP7LightSaber::OnAttackStartHandle()
{
	Super::OnAttackStartHandle();
	SwitchRibbon(true);
}

void AP7LightSaber::OnAttackEndHandle()
{
	Super::OnAttackEndHandle();
	SwitchRibbon(false);
}

void AP7LightSaber::Equip(USceneComponent* InParent, FName SocketName, AActor* NewOwner, APawn* NewInstigator)
{
	Super::Equip(InParent, SocketName, NewOwner, NewInstigator);
	SwitchRibbon(true);
	/* I prefer to let the weapon choose its color instead of the character */
	if (const AP7Enemy* EnemyOwner = Cast<AP7Enemy>(GetOwner()))
	{
		SetSaberColor(EnemyOwner->GetTeamColor());
	}
}

void AP7LightSaber::SetSaberColor(const FLinearColor NewColor)
{
	if (!Beam || !SaberLight) return;

	Color = NewColor;
	SaberColorMaterial = Beam->CreateAndSetMaterialInstanceDynamic(0);
	if (!SaberColorMaterial) return;

	SaberColorMaterial->SetVectorParameterValue(ColorParameterName, Color);
	SaberLight->SetLightColor(Color);

	if (RibbonComponent)
	{
		RibbonComponent->SetVariableLinearColor(Ribbon.ColorName, NewColor);
	}
}

void AP7LightSaber::SwitchRibbon(const bool bOn)
{
	if (RibbonComponent)
	{
		bOn ? RibbonComponent->Activate() : RibbonComponent->Deactivate();
		return;
	}
	/* Create niagara on the first try */
	if (!Ribbon.Effect) return;
	RibbonComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
		Ribbon.Effect, GetRootComponent(), NAME_None, FVector(0.f, 0.f, 61.f), FRotator::ZeroRotator, EAttachLocation::SnapToTarget, false);

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

void AP7LightSaber::TouchTrace()
{
	if (!bBladeOverlapped) return;
	FHitResult HitResult;
	SweepSphere(HitResult);
	/* Decals & splashes but without damage */
	if (!HitResult.bBlockingHit) return;
	SplashEffect(HitResult);
	PlasmaDecal(HitResult);
}

void AP7LightSaber::SweepSphere(FHitResult& HitResult)
{
	const FVector Start = TraceStart->GetComponentLocation();
	const FVector End = TraceEnd->GetComponentLocation();
	const FQuat Rotation = TraceStart->GetComponentRotation().Quaternion();

	FCollisionShape SphereShape;
	SphereShape.SetSphere(5.f);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	GetWorld()->SweepSingleByChannel(HitResult, Start, End, Rotation, ECC_Visibility, SphereShape, QueryParams);
}

void AP7LightSaber::PlasmaDecal(const FHitResult& HitResult)
{
	/* Generate more decal points */
	PlasmaContacts.Add(FP7PlasmaSpawn(HitResult));
	if (PlasmaContacts.Num() < 2) return;

	const int32 Distance = (PlasmaContacts[1].Location - PlasmaContacts[0].Location).Size();
	for (int i = 0; i < Distance / SpawnStep; ++i)
	{
		AllPlasmaDecalLocations.Add(FMath::VInterpTo(PlasmaContacts[0].Location, PlasmaContacts[1].Location, i, SpawnStep / Distance));
	}
	PlasmaContacts.RemoveAt(0);

	/* Spawn decals */
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
