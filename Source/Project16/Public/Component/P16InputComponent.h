// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "Project16.h"
#include "AbilitySystem/Data/P16InputConfig.h"
#include "P16InputComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT16_API UP16InputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
public:
	UP16InputComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* ------------------------------- This -------------------------------- */
public:
	template <typename UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(const UP16InputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
};

template <typename UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UP16InputComponent::BindAbilityActions(const UP16InputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	EARLY_RETURN_IF(!InputConfig)

	for (const FP16InputAction& InputAction : InputConfig->AbilityInputActions)
	{
		CONTINUE_IF(!InputAction.Action && InputAction.Tag.IsValid())

		if (PressedFunc)
		{
			BindAction(InputAction.Action, ETriggerEvent::Started, Object, PressedFunc, InputAction.Tag);
		}
		if (ReleasedFunc)
		{
			BindAction(InputAction.Action, ETriggerEvent::Completed, Object, ReleasedFunc, InputAction.Tag);
		}
		if (HeldFunc)
		{
			BindAction(InputAction.Action, ETriggerEvent::Triggered, Object, HeldFunc, InputAction.Tag);
		}
	}
}
