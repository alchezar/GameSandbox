// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "Project17.h"
#include "Data/P17Data_InputConfig.h"
#include "P17InputComponent.generated.h"

struct FGameplayTag;
class UP17Data_InputConfig;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT17_API UP17InputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
	template <typename UserObject, typename CallbackFunc>
	void BindNativeInputFunction(const UP17Data_InputConfig* InInputConfig, const FGameplayTag& InInputTag, ETriggerEvent InTriggerEvent, UserObject* Context, CallbackFunc Callback);

	template <typename UserObject, typename CallbackFunc>
	void BindAbilityInputFunction(const UP17Data_InputConfig* InInputConfig, UserObject* Context, CallbackFunc PressedCallback, CallbackFunc ReleasedCallback);
};

template <typename UserObject, typename CallbackFunc>
void UP17InputComponent::BindNativeInputFunction(const UP17Data_InputConfig* InInputConfig, const FGameplayTag& InInputTag, ETriggerEvent InTriggerEvent, UserObject* Context, CallbackFunc Callback)
{
	WARN_RETURN_IF(!InInputConfig,);
	UInputAction* InputAction = InInputConfig->FindNativeInputAction(InInputTag);
	WARN_RETURN_IF(!InputAction,);

	BindAction(InputAction, InTriggerEvent, Context, Callback);
}

template <typename UserObject, typename CallbackFunc>
void UP17InputComponent::BindAbilityInputFunction(const UP17Data_InputConfig* InInputConfig, UserObject* Context, CallbackFunc PressedCallback, CallbackFunc ReleasedCallback)
{
	WARN_RETURN_IF(!InInputConfig,);

	for (const FP17InputActionConfig& Config : InInputConfig->AbilityInputActions)
	{
		CONTINUE_IF(!Config.IsValid())
		BindAction(Config.Action, ETriggerEvent::Started, Context, PressedCallback, Config.Tag);
		BindAction(Config.Action, ETriggerEvent::Completed, Context, ReleasedCallback, Config.Tag);
	}
}
