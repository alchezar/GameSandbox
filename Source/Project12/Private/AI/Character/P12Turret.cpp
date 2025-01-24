// Copyright (C) 2023, IKinder

#include "AI/Character/P12Turret.h"

#include "AIController.h"
#include "NiagaraFunctionLibrary.h"
#include "Component/Scene/P12WeaponBarrelComponent.h"
#include "Net/UnrealNetwork.h"
#include "Player/P12BaseCharacter.h"

AP12Turret::AP12Turret()
{
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);

	USceneComponent* TurretRoot = CreateDefaultSubobject<USceneComponent>("TurretRootComponent");
	SetRootComponent(TurretRoot);

	TurretBaseComponent = CreateDefaultSubobject<USceneComponent>("TurretBaseSceneComponent");
	TurretBaseComponent->SetupAttachment(RootComponent);

	TurretBarrelComponent = CreateDefaultSubobject<USceneComponent>("TurretBarrelSceneComponent");
	TurretBarrelComponent->SetupAttachment(TurretBaseComponent);

	BarrelComponent = CreateDefaultSubobject<UP12WeaponBarrelComponent>("TurretWeaponBarrelComponent");
	BarrelComponent->SetupAttachment(TurretBarrelComponent);
}

void AP12Turret::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TurretState == EP12TurretState::Searching)
	{
		SearchingMovement(DeltaTime);
	}
	else if (TurretState == EP12TurretState::Firing)
	{
		FiringMovement(DeltaTime);
	}
}

void AP12Turret::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, CurrentTarget);
}

FVector AP12Turret::GetPawnViewLocation() const
{
	return BarrelComponent->GetComponentLocation();
}

FRotator AP12Turret::GetViewRotation() const
{
	return BarrelComponent->GetComponentRotation();
}

void AP12Turret::SetCurrentTarget(AActor* NewTarget)
{
	if (CurrentTarget != NewTarget)
	{
		BindOnTargetHealthChanged(CurrentTarget, false);
	}
	CurrentTarget = NewTarget;
	BindOnTargetHealthChanged(CurrentTarget, true);

	OnCurrentTargetSet();
}

inline void AP12Turret::OnCurrentTargetSet()
{
	const EP12TurretState NewState = CurrentTarget ? EP12TurretState::Firing : EP12TurretState::Searching;
	SetCurrentTurretState(NewState);
}

void AP12Turret::SetCurrentTurretState(const EP12TurretState NewState)
{
	if (TurretState == NewState)
	{
		return;
	}
	TurretState = NewState;

	if (TurretState == EP12TurretState::Searching)
	{
		GetWorld()->GetTimerManager().ClearTimer(ShotTimer);
	}
	if (TurretState == EP12TurretState::Firing)
	{
		GetWorld()->GetTimerManager().SetTimer(ShotTimer, this, &ThisClass::MakeShot, GetFireInterval(), true, FireDelayTime);
	}
}

void AP12Turret::SearchingMovement(const float DeltaTime)
{
	FRotator TurretBaseRotation = TurretBaseComponent->GetRelativeRotation();
	TurretBaseRotation.Yaw += DeltaTime * BaseSearchingRotationRate;
	TurretBaseComponent->SetRelativeRotation(TurretBaseRotation);

	FRotator TurretBarrelRotation = TurretBarrelComponent->GetRelativeRotation();
	TurretBarrelRotation.Pitch = FMath::FInterpTo(TurretBarrelRotation.Pitch, 0.f, DeltaTime, BarrelPitchRotationRate);
	TurretBarrelComponent->SetRelativeRotation(TurretBarrelRotation);
}

void AP12Turret::FiringMovement(const float DeltaTime)
{
	const FVector LookAtDirection = (CurrentTarget->GetActorLocation() - TurretBaseComponent->GetComponentLocation()).GetSafeNormal2D();
	const FQuat LookAtQuat = LookAtDirection.ToOrientationQuat();
	const FQuat TargetQuat = FMath::QInterpTo(TurretBaseComponent->GetComponentQuat(), LookAtQuat, DeltaTime, BaseFiringInterpSpeed);
	TurretBaseComponent->SetWorldRotation(TargetQuat);

	const FVector LookDownDirection = (CurrentTarget->GetActorLocation() - TurretBarrelComponent->GetComponentLocation()).GetSafeNormal();
	const FRotator LookAtRotation = LookDownDirection.ToOrientationRotator();
	const float ClampedPitch = FMath::RInterpTo(TurretBarrelComponent->GetComponentRotation(), LookAtRotation, DeltaTime, BaseFiringInterpSpeed).Pitch;
	const FRotator ClampedRotation = FRotator(FMath::Clamp(ClampedPitch, BarrelPitchAngle.Min, BarrelPitchAngle.Max), 0.f, 0.f);
	TurretBarrelComponent->SetRelativeRotation(ClampedRotation);
}

float AP12Turret::GetFireInterval() const
{
	return 60.f / RateOfFire;
}

void AP12Turret::MakeShot()
{
	const FVector BarrelLocation = BarrelComponent->GetComponentLocation();
	if (MuzzleNiagara)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), MuzzleNiagara, BarrelLocation);
	}
	BarrelComponent->Shot(BarrelLocation, BarrelComponent->GetForwardVector(), BulletSpread);
}

void AP12Turret::BindOnTargetHealthChanged(AActor* Target, const bool bBind)
{
	AP12BaseCharacter* BaseChar = Cast<AP12BaseCharacter>(Target);
	if (!BaseChar)
	{
		return;
	}
	if (!bBind && BaseChar->OnHealthChange.IsBound())
	{
		BaseChar->OnHealthChange.RemoveAll(this);
		return;
	}
	BaseChar->OnHealthChange.AddUObject(this, &ThisClass::OnTargetHealthChangedHandle);
}

void AP12Turret::OnTargetHealthChangedHandle(float Health, float MaxHealth)
{
	if (Health <= 0.f)
	{
		SetCurrentTurretState(EP12TurretState::Searching);
		SetCurrentTarget(nullptr);
	}
}

void AP12Turret::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (AAIController* AIController = Cast<AAIController>(NewController))
	{
		const FGenericTeamId TeamID = {static_cast<uint8>(Team)};
		AIController->SetGenericTeamId(TeamID);
	}
}

void AP12Turret::OnRep_CurrentTarget()
{
	OnCurrentTargetSet();
}
