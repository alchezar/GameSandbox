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
		UP16AbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent);
	}
	InitHealthBar();
	InitHitReact();
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

void AP16Enemy::Die()
{
	SetLifeSpan(LifeSpan);
	Super::Die();
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
		.AddWeakLambda(this, [this](const FOnAttributeChangeData& Data) -> void {
			OnHealthChanged.Broadcast(Data.NewValue);
		});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(EnemyAttributeSet->GetMaxHealthAttribute())
		.AddWeakLambda(this, [this](const FOnAttributeChangeData& Data) -> void {
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		});

	// Broadcast initial values.
	OnHealthChanged.Broadcast(EnemyAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(EnemyAttributeSet->GetMaxHealth());
}

void AP16Enemy::InitHitReact()
{
	BaseWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;

	AbilitySystemComponent->RegisterGameplayTagEvent(FGSGameplayTagsSingleton::Get().P16Tags.Effect.HitReactTag)
		.AddUObject(this, &ThisClass::OnHitReactCallback);
}

void AP16Enemy::OnHitReactCallback(const FGameplayTag Tag, const int32 Count)
{
	bHitReacting = Count > 0;

	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
	AIController->GetBlackboardComponent()->SetValueAsBool("HitReacting", bHitReacting);
}
