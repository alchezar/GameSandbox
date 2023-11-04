// Copyright (C) 2023, IKinder

#include "P12/Public/Component/Actor/P12AttributeComponent.h"

#include "P12/Public/Player/P12BaseCharacter.h"
#include "P12/Public/Util/P12Library.h"

UP12AttributeComponent::UP12AttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UP12AttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	check(GetOwner()->IsA<AP12BaseCharacter>())
	CachedCharacterOwner = StaticCast<AP12BaseCharacter*>(GetOwner());
	CachedCharacterOwner->OnTakeAnyDamage.AddDynamic(this, &ThisClass::OnTakeAnyDamage);
	
	Health = MaxHealth;
	CachedCharacterOwner->OnHealthChange.Broadcast(Health, MaxHealth);
}

void UP12AttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
	DebugDrawAttributes();
#endif

}

void UP12AttributeComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (!GetIsAlive())
	{
		return;
	}
	
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
	CachedCharacterOwner->OnHealthChange.Broadcast(Health, MaxHealth);
	UP12Library::DrawPrintString(GetWorld(), FString::Printf(TEXT("AP12BaseCharacter %s received %.2f amount of damage from %s"), *CachedCharacterOwner->GetName(), Damage, *DamageCauser->GetName()), UP12Library::GetCanDrawDebugPrintScreen());

	if (FMath::IsNearlyZero(Health) && OnDeath.IsBound())
	{
		OnDeath.Broadcast();
		UP12Library::DrawPrintString(GetWorld(), FString::Printf(TEXT("AP12BaseCharacter %s killed by %s"), *CachedCharacterOwner->GetName(), *DamageCauser->GetName()), UP12Library::GetCanDrawDebugPrintScreen());
	}
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
