// FCopyright (C) 2023, IKinder

#include "GameSandbox/P1/PickUp/PickUpBattery.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

#pragma region Default

APickUpBattery::APickUpBattery()
{
	PrimaryActorTick.bCanEverTick = true;

	PickupMesh->SetSimulatePhysics(true);
}

void APickUpBattery::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle DestroyTimer;
	GetWorld()->GetTimerManager().SetTimer(OUT DestroyTimer, this, &APickUpBattery::DestroyItself, Lifetime);
}

void APickUpBattery::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#pragma endregion // Default

#pragma region Charge

float APickUpBattery::GetChargeAmount() const
{
	return ChargeAmount;
}

void APickUpBattery::OnPickupCollect_Implementation()
{
	Super::OnPickupCollect_Implementation();

	FTimerHandle EmitterTimer;
	GetWorldTimerManager().SetTimer(EmitterTimer, this, &APickUpBattery::CreateEmitter, 0.3f, false);
}

void APickUpBattery::CreateEmitter()
{
	Emitter = UGameplayStatics::SpawnEmitterAttached(
		Effect, PickupMesh, NAME_None,
		FVector(ForceInit), FRotator::ZeroRotator, FVector(1),
		EAttachLocation::SnapToTargetIncludingScale);
	UpdateTarget();
	FTimerHandle TargetTimer;
	GetWorldTimerManager().SetTimer(TargetTimer, this, &ThisClass::UpdateTarget, 0.1f, true);
	FTimerHandle DestroyTimer;
	GetWorldTimerManager().SetTimer(DestroyTimer, this, &ThisClass::DestroyItself, 0.8f, false);
}

void APickUpBattery::UpdateTarget() const
{
	const FVector TargetLocation = UGameplayStatics::GetPlayerCharacter(this, 0)->GetMesh()->GetSocketLocation("hand_r");
	if (Emitter)
	{
		Emitter->SetBeamTargetPoint(1, TargetLocation, 0);
	}
}

void APickUpBattery::DestroyItself()
{
	Destroy();
}

#pragma endregion // Charge
