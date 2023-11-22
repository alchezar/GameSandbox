// Copyright (C) 2023, IKinder

#include "P12/Public/Component/Actor/P12AttributeComponent.h"

#include "Net/UnrealNetwork.h"
#include "P12/Public/Player/P12BaseCharacter.h"
#include "P12/Public/Util/P12Library.h"

UP12AttributeComponent::UP12AttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UP12AttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	check(GetOwner()->IsA<AP12BaseCharacter>())
	CachedCharacterOwner = StaticCast<AP12BaseCharacter*>(GetOwner());
	CachedCharacterOwner->OnTakeAnyDamage.AddDynamic(this, &ThisClass::OnTakeAnyDamage);
	
	Health = MaxHealth;
	if (GetOwner()->HasAuthority())
	{
		OnHealthChanged();
	}
}

void UP12AttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
	DebugDrawAttributes();
#endif

}

void UP12AttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Health);
}

void UP12AttributeComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (!GetIsAlive())
	{
		return;
	}
	
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
	OnHealthChanged();

	CheckIfDead();
}

#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
void UP12AttributeComponent::DebugDrawAttributes()
{
	// UP12DebugSubsystem* DebugSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UP12DebugSubsystem>();
	if (!CachedCharacterOwner.IsValid())
	{
		return;
	}
	const FVector TextLocation = CachedCharacterOwner->GetActorLocation() + 100.f * FVector::UpVector;
	const FString Text = "Health: " + FString::SanitizeFloat(Health);
	UP12Library::DrawDebugText(GetWorld(), TextLocation, Text, UP12Library::GetCanDrawDebugText(), true);
}
#endif

float UP12AttributeComponent::GetHealthPercent()
{
	if (FMath::IsNearlyZero(MaxHealth))
	{
		return 0.f;
	}
	return Health / MaxHealth;
}

void UP12AttributeComponent::CheckIfDead()
{
	if (FMath::IsNearlyZero(Health) && OnDeath.IsBound())
	{
		OnDeath.Broadcast();
	}
}

void UP12AttributeComponent::OnRep_Health(float LastHealth)
{
	if (!CachedCharacterOwner.IsValid())
	{
		CachedCharacterOwner = Cast<AP12BaseCharacter>(GetOwner());
		check(CachedCharacterOwner.Get());
	}
	OnHealthChanged();

	CheckIfDead();
}

void UP12AttributeComponent::OnHealthChanged()
{
	CachedCharacterOwner->OnHealthChange.Broadcast(Health, MaxHealth);
}

void UP12AttributeComponent::AddHealth(const float HealthToAdd)
{
	Health = FMath::Clamp(Health + HealthToAdd, 0, MaxHealth);
	OnHealthChanged();
}

void UP12AttributeComponent::RestoreFullStamina()
{
	
}
