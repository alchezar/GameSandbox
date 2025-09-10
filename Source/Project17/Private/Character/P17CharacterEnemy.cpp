// Copyright © 2025, Ivan Kinder

#include "Character/P17CharacterEnemy.h"

#include "Component/Combat/P17CombatEnemyComponent.h"
#include "Component/UI/P17UIEnemyComponent.h"
#include "Components/WidgetComponent.h"
#include "Data/P17Data_StartupBase.h"
#include "Engine/AssetManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Widget/P17WidgetBase.h"

AP17CharacterEnemy::AP17CharacterEnemy()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	if (auto* Movement = GetCharacterMovement())
	{
		Movement->bUseControllerDesiredRotation = false;
		Movement->bOrientRotationToMovement = true;
		Movement->RotationRate = {0.f, 180.f, 0.f};
		Movement->MaxWalkSpeed = 300.f;
		Movement->BrakingDecelerationWalking = 1000.f;
	}

	CombatComponent = CreateDefaultSubobject<UP17CombatEnemyComponent>("EnemyCombatComponent");
	UIComponent = CreateDefaultSubobject<UP17UIEnemyComponent>("EnemyUIComponent");
	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("EnemyHealthWidgetComponent");
	HealthWidgetComponent->SetupAttachment(GetMesh());
}

void AP17CharacterEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Load startup data asynchronously.
	UAssetManager::GetStreamableManager().RequestAsyncLoad(
		StartupData.ToSoftObjectPath(),
		FStreamableDelegate::CreateUObject(this, &ThisClass::InitEnemyStartupData));
}

void AP17CharacterEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (UUserWidget* Widget = HealthWidgetComponent->GetUserWidgetObject())
	{
		UP17WidgetBase* HealthWidget = Cast<UP17WidgetBase>(Widget);
		WARN_RETURN_IF(!HealthWidget,)

		HealthWidget->InitEnemyCreatedWidget(this);
	}
}

UP17CombatPawnComponent* AP17CharacterEnemy::GetCombatComponent() const
{
	return GetEnemyCombatComponent();
}

UP17UIPawnComponent* AP17CharacterEnemy::GetPawnUIComponent() const
{
	return GetUIComponent();
}

UP17UIEnemyComponent* AP17CharacterEnemy::GetEnemyUIComponent() const
{
	return GetUIComponent();
}

void AP17CharacterEnemy::InitEnemyStartupData() const
{
	UP17Data_StartupBase* LoadedData = StartupData.Get();
	WARN_RETURN_IF(!LoadedData,)
	LoadedData->GiveToAbilitySystemComponent(GetProjectAbilitySystemComponent());
}
