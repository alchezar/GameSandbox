// Copyright © 2025, Ivan Kinder

#include "Widget/P17WidgetBase.h"

#include "Project17.h"
#include "Interface/P17UIInterface.h"

void UP17WidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	const TScriptInterface<IP17UIInterface> UIInterface = GetOwningPlayerPawn();
	RETURN_IF(!UIInterface,)
	UP17UIHeroComponent* HeroUIComponent = UIInterface->GetHeroUIComponent();
	WARN_RETURN_IF(!HeroUIComponent,)

	BP_OnOwningHeroUIComponentInit(HeroUIComponent);
}

void UP17WidgetBase::InitEnemyCreatedWidget(AActor* OwningEnemyActor)
{
	const TScriptInterface<IP17UIInterface> UIInterface = OwningEnemyActor;
	RETURN_IF(!UIInterface,)
	UP17UIEnemyComponent* EnemyUIComponent = UIInterface->GetEnemyUIComponent();
	WARN_RETURN_IF(!EnemyUIComponent,)

	BP_OnOwningEnemyUIComponentInit(EnemyUIComponent);
}
