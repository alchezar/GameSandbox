// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/P16AbilitySystemLibrary.h"

#include "Project16.h"
#include "Kismet/GameplayStatics.h"
#include "Player/P16PlayerState.h"
#include "UI/HUD/P16HUD.h"

UP16OverlayWidgetController* UP16AbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	EARLY_RETURN_VALUE_IF(!WorldContextObject, nullptr)

	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	EARLY_RETURN_VALUE_IF(!PlayerController, nullptr)

	AP16HUD* HUD = PlayerController->GetHUD<AP16HUD>();
	EARLY_RETURN_VALUE_IF(!HUD, nullptr)

	return HUD->GetOverlayWidgetController(GetWidgetControllerParams(WorldContextObject));
}

UP16AttributeMenuWidgetController* UP16AbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	EARLY_RETURN_VALUE_IF(!WorldContextObject, nullptr)

	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	EARLY_RETURN_VALUE_IF(!PlayerController, nullptr)

	AP16HUD* HUD = PlayerController->GetHUD<AP16HUD>();
	EARLY_RETURN_VALUE_IF(!HUD, nullptr)

	return HUD->GetAttributeMenuWidgetController(GetWidgetControllerParams(WorldContextObject));
}

FP16WidgetControllerParams UP16AbilitySystemLibrary::GetWidgetControllerParams(const UObject* WorldContextObject)
{
	FP16WidgetControllerParams Result = {};
	EARLY_RETURN_VALUE_IF(!WorldContextObject, Result)

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	EARLY_RETURN_VALUE_IF(!PlayerController, Result)

	const AP16HUD*   HUD         = PlayerController->GetHUD<AP16HUD>();
	AP16PlayerState* PlayerState = PlayerController->GetPlayerState<AP16PlayerState>();
	EARLY_RETURN_VALUE_IF(!HUD || !PlayerState, Result)

	UAbilitySystemComponent* AbilitySystem = PlayerState->GetAbilitySystemComponent();
	UAttributeSet*           AttributeSet  = PlayerState->GetAttributeSet();

	Result = FP16WidgetControllerParams{PlayerController, PlayerState, AbilitySystem, AttributeSet};
	return Result;
}
