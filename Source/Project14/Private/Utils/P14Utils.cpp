// Copyright © 2025, Ivan Kinder

#include "Utils/P14Utils.h"

#include "EnhancedPlayerInput.h"
#include "Misc/OutputDeviceNull.h"

FString P14::Test::GetTestDataFullPath()
{
	return FPaths::Combine(FPaths::ProjectDir(), "Data", "Project14", "CharacterInputData.json");
}

int32 P14::Test::GetActionBindingIndex(const UEnhancedInputComponent* InInputComp, const FString& InActionName, const ETriggerEvent InTriggerEvent)
{
	int32 Result = INDEX_NONE;
	if (!InInputComp)
	{
		return Result;
	}

	for (int32 Index = 0; Index < InInputComp->GetActionEventBindings().Num(); ++Index)
	{
		const UInputAction* InputAction  = InInputComp->GetActionEventBindings()[Index]->GetAction();
		const ETriggerEvent TriggerEvent = InInputComp->GetActionEventBindings()[Index]->GetTriggerEvent();

		if (InputAction->GetName() == InActionName && TriggerEvent == InTriggerEvent)
		{
			Result = Index;
			break;
		}
	}

	return Result;
}

void P14::Test::CallBlueprintFuncByNameWithParams(UObject* InObject, const FString& InFuncName, const TArray<FString>& InParams)
{
	if (!InObject)
	{
		return;
	}

	FString Command = InFuncName;
	for (const FString& Param : InParams)
	{
		Command.Append(" ").Append(Param);
	}

	FOutputDeviceNull OutputDeviceNull{};
	InObject->CallFunctionByNameWithArguments(*Command, OutputDeviceNull, nullptr, true);
}

void P14::Test::InjectActionInput(const APlayerController* InController, const FString& InActionName, FInputActionValue&& InActionValue)
{
	check(InController)
	const UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(InController->InputComponent);
	check(EnhancedInputComp)

	const TUniquePtr<FEnhancedInputActionEventBinding>* FoundActionBinding = EnhancedInputComp->GetActionEventBindings().FindByPredicate(
		[InActionName](const TUniquePtr<FEnhancedInputActionEventBinding>& ActionEvent) -> bool
		{
			return ActionEvent->GetAction()->GetName() == InActionName;
		});
	check(FoundActionBinding)

	const UInputAction* InputAction = (*FoundActionBinding)->GetAction();
	check(InputAction)

	UEnhancedPlayerInput* EnhancedInput = Cast<UEnhancedPlayerInput>(InController->PlayerInput);
	check(EnhancedInput)

	EnhancedInput->InjectInputForAction(InputAction, InActionValue, InputAction->Modifiers, InputAction->Triggers);
}
