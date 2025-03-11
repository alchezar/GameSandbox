// Copyright © 2025, Ivan Kinder

#include "Character/P16Enemy.h"

#include "Project16.h"
#include "AbilitySystem/P16AbilitySystemComponent.h"
#include "AbilitySystem/P16AttributeSet.h"
#include "Components/WidgetComponent.h"

AP16Enemy::AP16Enemy()
{
	PrimaryActorTick.bCanEverTick = true;

	// For enemies highlight on hover.
	GetMesh()->SetCustomDepthStencilValue(P16::CustomDepthRed);
	Weapon->SetCustomDepthStencilValue(P16::CustomDepthRed);

	AbilitySystemComponent = CreateDefaultSubobject<UP16AbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UP16AttributeSet>("AttributeSetComponent");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBarWidgetComponent");
	HealthBar->SetupAttachment(GetRootComponent());
}

void AP16Enemy::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();
	InitHealthBar();
}

void AP16Enemy::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP16Enemy::ToggleHighlight(const bool bOn)
{
	GetMesh()->SetRenderCustomDepth(bOn);
	Weapon->SetRenderCustomDepth(bOn);
}

void AP16Enemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	Super::InitAbilityActorInfo();
}

void AP16Enemy::InitHealthBar()
{
	// Set the enemy as the widget controller for the health bar.
	UP16Widget* HealthBarWidget = Cast<UP16Widget>(HealthBar->GetUserWidgetObject());
	EARLY_RETURN_IF(!HealthBarWidget)
	HealthBarWidget->SetWidgetController(this);

	// Listen for health changes.
	const UP16AttributeSet* EnemyAttributeSet = Cast<UP16AttributeSet>(AttributeSet);
	EARLY_RETURN_IF(!EnemyAttributeSet)
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(EnemyAttributeSet->GetHealthAttribute())
		.AddWeakLambda(this, [this](const FOnAttributeChangeData& Data) -> void
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(EnemyAttributeSet->GetMaxHealthAttribute())
		.AddWeakLambda(this, [this](const FOnAttributeChangeData& Data) -> void
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		});

	// Broadcast initial values.
	OnHealthChanged.Broadcast(EnemyAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(EnemyAttributeSet->GetMaxHealth());
}
