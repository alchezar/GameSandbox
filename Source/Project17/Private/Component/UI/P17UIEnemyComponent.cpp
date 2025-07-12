// Copyright © 2025, Ivan Kinder

#include "Component/UI/P17UIEnemyComponent.h"

#include "Widget/P17WidgetBase.h"

UP17UIEnemyComponent::UP17UIEnemyComponent()
{}

void UP17UIEnemyComponent::RegisterEnemyDrawWidget(UP17WidgetBase* InWidgetToRegister)
{
	RETURN_IF(!InWidgetToRegister,)
	EnemyDrawnWidgets.AddUnique(InWidgetToRegister);
}

void UP17UIEnemyComponent::RemoveEnemyDrawWidgets()
{
	RETURN_IF(EnemyDrawnWidgets.IsEmpty(),)
	for (UP17WidgetBase* EnemyDrawnWidget : EnemyDrawnWidgets)
	{
		CONTINUE_IF(!EnemyDrawnWidget)
		EnemyDrawnWidget->RemoveFromParent();
		EnemyDrawnWidget = nullptr;
	}
	EnemyDrawnWidgets.Empty();
}
