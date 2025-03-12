// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/P16AbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "Project16.h"
#include "AbilitySystem/Data/P16CharacterClassInfoDataAsset.h"
#include "Game/P16GameMode.h"
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

void UP16AbilitySystemLibrary::InitDefaultAttributes(const UObject* WorldContextObject, const EP16CharacterClass CharacterClass, UAbilitySystemComponent* AbilitySystemComponent, const float Level)
{
	EARLY_RETURN_IF(!WorldContextObject || !AbilitySystemComponent)
	const AP16GameMode* GameMode = Cast<AP16GameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	EARLY_RETURN_IF(!GameMode)
	UP16CharacterClassInfoDataAsset* ClassInfo = GameMode->GetCharacterClassInfo();
	EARLY_RETURN_IF(!ClassInfo)

	const AActor*                       Avatar      = AbilitySystemComponent->GetAvatarActor();
	const FP16CharacterClassDefaultInfo DefaultInfo = ClassInfo->GetClassDefaultInfo(CharacterClass);

	ApplyGameplayEffect(AbilitySystemComponent, DefaultInfo.PrimaryAttributes, Level, Avatar);
	ApplyGameplayEffect(AbilitySystemComponent, ClassInfo->SecondaryAttributes, Level, Avatar);
	ApplyGameplayEffect(AbilitySystemComponent, ClassInfo->VitalAttributes, Level, Avatar);
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

void UP16AbilitySystemLibrary::ApplyGameplayEffect(UAbilitySystemComponent* AbilitySystemComponent, const TSubclassOf<UGameplayEffect>& GameplayEffect, const float Level, const AActor* SourceObject)
{
	FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
	ContextHandle.AddSourceObject(SourceObject);

	const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, Level, ContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}
