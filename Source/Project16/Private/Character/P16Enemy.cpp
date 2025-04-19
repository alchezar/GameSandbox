// Copyright © 2025, Ivan Kinder

#include "Character/P16Enemy.h"

#include "Project16.h"
#include "AbilitySystem/P16AbilitySystemComponent.h"
#include "AbilitySystem/P16AbilitySystemLibrary.h"
#include "AbilitySystem/P16AttributeSet.h"
#include "AI/P16AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Root/Public/Singleton/GSGameplayTagsSingleton.h"

AP16Enemy::AP16Enemy()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw   = false;
	bUseControllerRotationRoll  = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = true;

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

void AP16Enemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	EARLY_RETURN_IF(!HasAuthority())
	AIController                              = Cast<AP16AIController>(NewController);
	UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent();
	EARLY_RETURN_IF(!BlackboardComponent)
	BlackboardComponent->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	AIController->RunBehaviorTree(BehaviorTree);
	BlackboardComponent->SetValueAsBool("HitReacting", false);
	BlackboardComponent->SetValueAsBool("RangeAttacker", CharacterClass != EP16CharacterClass::Warrior);
}

void AP16Enemy::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();
	if (HasAuthority())
	{
		UP16AbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent, CharacterClass);
	}
	InitHealthBar();
	InitHitReact();
}

void AP16Enemy::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

int32 AP16Enemy::GetPlayerLevel_Implementation()
{
	return Level;
};

void AP16Enemy::Die(const FVector& DeathImpulse)
{
	SetLifeSpan(LifeSpan);
	if (AIController)
	{
		AIController->GetBlackboardComponent()->SetValueAsBool("Dead", true);
	}
	Super::Die(DeathImpulse);
}

void AP16Enemy::ToggleHighlight(const bool bOn)
{
	GetMesh()->SetRenderCustomDepth(bOn);
	Weapon->SetRenderCustomDepth(bOn);
}

AActor* AP16Enemy::GetCombatTarget_Implementation() const
{
	return CombatTarget;
}

void AP16Enemy::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

void AP16Enemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	EARLY_RETURN_IF(!HasAuthority())

	Super::InitAbilityActorInfo();
}

void AP16Enemy::InitDefaultAttributes() const
{
	UP16AbilitySystemLibrary::InitDefaultAttributes(this, CharacterClass, AbilitySystemComponent.Get(), Level);
}

void AP16Enemy::OnStunTagChanged(const FGameplayTag Tag, const int32 Count)
{
	Super::OnStunTagChanged(Tag, Count);

	EARLY_RETURN_IF(!AIController || !AIController->GetBlackboardComponent())
	AIController->GetBlackboardComponent()->SetValueAsBool("Stunned", bStunned);
}

void AP16Enemy::InitHealthBar()
{
	// Set the enemy as the widget controller for the health bar.
	UP16Widget* HealthBarWidget = Cast<UP16Widget>(HealthBar->GetUserWidgetObject());
	EARLY_RETURN_IF(!HealthBarWidget)
	HealthBarWidget->SetWidgetController(this);

	// Listen for health changes.
	// clang-format off
	const UP16AttributeSet* EnemyAttributeSet = Cast<UP16AttributeSet>(AttributeSet);
	EARLY_RETURN_IF(!EnemyAttributeSet)
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(EnemyAttributeSet->GetHealthAttribute())
		.AddWeakLambda(this, [this](const FOnAttributeChangeData& Data) -> void { OnHealthChanged.Broadcast(Data.NewValue); });
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(EnemyAttributeSet->GetMaxHealthAttribute())
		.AddWeakLambda(this, [this](const FOnAttributeChangeData& Data) -> void { OnMaxHealthChanged.Broadcast(Data.NewValue); });
	// clang-format on

	// Broadcast initial values.
	OnHealthChanged.Broadcast(EnemyAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(EnemyAttributeSet->GetMaxHealth());
}

void AP16Enemy::InitHitReact()
{
	AbilitySystemComponent->RegisterGameplayTagEvent(FGSGameplayTagsSingleton::Get().P16Tags.Effect.HitReactTag)
		.AddUObject(this, &ThisClass::OnHitReactCallback);
}

void AP16Enemy::OnHitReactCallback(const FGameplayTag Tag, const int32 Count)
{
	const bool bHitReacting = Count > 0;

	GetCharacterMovement()->MaxWalkSpeed = bHitReacting || bStunned ? 0.f : BaseWalkSpeed;
	EARLY_RETURN_IF(!AIController || !AIController->GetBlackboardComponent())

	AIController->GetBlackboardComponent()->SetValueAsBool("HitReacting", bHitReacting);
}
