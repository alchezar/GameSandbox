// Copyright (C) 2023, IKinder

#include "STU_HealthComponent.h"
#include "STU_GameModeBase.h"
#include "Camera/CameraShakeBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Perception/AISense_Damage.h"

USTU_HealthComponent::USTU_HealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USTU_HealthComponent::BeginPlay()
{
	Super::BeginPlay();
	check(MaxHealth > 0);

	SetHealth(MaxHealth);

	AActor* Owner = GetOwner();
	if (!Owner) return;
	Owner->OnTakeAnyDamage.AddDynamic(this, &USTU_HealthComponent::OnTakeAnyDamageHandle);
	Owner->OnTakePointDamage.AddDynamic(this, &USTU_HealthComponent::OnTakePointDamage);
	Owner->OnTakeRadialDamage.AddDynamic(this, &USTU_HealthComponent::OnTakeRadialDamage);
}

float USTU_HealthComponent::GetHealth() const
{
	return Health;
}

bool USTU_HealthComponent::TryToAddHealth(const float PickedHealth)
{
	if (IsDead() || IsHealthFull()) return false;

	SetHealth(Health + PickedHealth);
	return true;
}

bool USTU_HealthComponent::IsHealthFull() const
{
	return FMath::IsNearlyEqual(Health, MaxHealth);
}

void USTU_HealthComponent::OnTakeAnyDamageHandle(AActor* DamagedActor, const float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{ }

void USTU_HealthComponent::OnTakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser)
{
	const float FinalDamage = Damage * GetPointDamageModifier(DamagedActor, BoneName);	
	ApplyDamage(FinalDamage, InstigatedBy);
}

void USTU_HealthComponent::OnTakeRadialDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, FVector Origin, const FHitResult& HitInfo, AController* InstigatedBy, AActor* DamageCauser)
{	
	ApplyDamage(Damage, InstigatedBy);
}

void USTU_HealthComponent::ApplyDamage(const float Damage, const AController* InstigatedBy)
{	
	if (Damage <= 0.f || IsDead() || !GetWorld()) return;
	SetHealth(Health - Damage);
	OnHealthChanged.Broadcast(Health, -Damage);

	if (IsDead())
	{
		Killed(InstigatedBy);
		OnDeath.Broadcast();
		GetWorld()->GetTimerManager().ClearTimer(OUT HealTimer);
	}
	else if (AutoHeal)
	{
		GetWorld()->GetTimerManager().SetTimer(OUT HealTimer, this, &ThisClass::Healing, HealUpdateDelay, true, HealStartDelay);
	}
	PlayCameraShake();
	ReportDamageEvent(Damage, InstigatedBy);
}

float USTU_HealthComponent::GetPointDamageModifier(AActor* Victim, const FName& BoneName)
{
	const ACharacter* Character = Cast<ACharacter>(Victim);
	if (!Character || !Character->GetMesh() || !Character->GetMesh()->GetBodyInstance(BoneName)) return 1.f;

	const UPhysicalMaterial* PhysicalMaterial = Character->GetMesh()->GetBodyInstance(BoneName)->GetSimplePhysicalMaterial();
	if (!DamageModifiers.Contains(PhysicalMaterial)) return 1.f;

	return DamageModifiers[PhysicalMaterial];
}

bool USTU_HealthComponent::IsDead() const
{
	return FMath::IsNearlyZero(Health);
}

float USTU_HealthComponent::GetHealthPercent() const
{
	return Health / MaxHealth;
}

void USTU_HealthComponent::SetHealth(const float NewHealth)
{
	const float NextHealth  = FMath::Clamp(NewHealth, 0.f, MaxHealth);
	const float HealthDelta = NextHealth - Health;

	Health = NextHealth;
	OnHealthChanged.Broadcast(Health, HealthDelta);
}

void USTU_HealthComponent::Healing()
{
	SetHealth(Health + HealAmount);
	if (IsHealthFull())
	{
		GetWorld()->GetTimerManager().ClearTimer(OUT HealTimer);
	}
}

void USTU_HealthComponent::PlayCameraShake() const
{
	if (IsDead()) return;

	const APawn* Player = Cast<APawn>(GetOwner());
	if (!Player) return;

	const APlayerController* Controller = Player->GetController<APlayerController>();
	if (!Controller || !Controller->PlayerCameraManager) return;

	Controller->PlayerCameraManager->StartCameraShake(CameraShake);
}

void USTU_HealthComponent::Killed(const AController* KillerController) const
{
	if (!GetWorld()) return;

	ASTU_GameModeBase* GameMode = Cast<ASTU_GameModeBase>(GetWorld()->GetAuthGameMode());
	if (!GameMode) return;

	const APawn* Player                 = Cast<APawn>(GetOwner());
	const AController* VictimController = Player ? Player->GetController() : nullptr;

	GameMode->Killed(KillerController, VictimController);
}

void USTU_HealthComponent::ReportDamageEvent(const float Damage, const AController* InstigatedBy)
{
	if (!GetWorld() || !GetOwner() || !InstigatedBy || !InstigatedBy->GetPawn()) return;
	UAISense_Damage::ReportDamageEvent(GetWorld(), GetOwner(), InstigatedBy->GetPawn(), Damage, InstigatedBy->GetPawn()->GetActorLocation(), GetOwner()->GetActorLocation());
}