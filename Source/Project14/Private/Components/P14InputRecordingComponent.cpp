// Copyright © 2024, Ivan Kinder

#include "Components/P14InputRecordingComponent.h"

#include "EnhancedInputComponent.h"
#include "Utils/P14JsonUtils.h"

UP14InputRecordingComponent::UP14InputRecordingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UP14InputRecordingComponent::TickComponent(const float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	SaveBindingsData();
}

void UP14InputRecordingComponent::BeginPlay()
{
	Super::BeginPlay();

	check(GetOwner())
	check(GetWorld())
	check(GetOwner()->InputComponent)
	check(Cast<UEnhancedInputComponent>(GetOwner()->InputComponent))

	EnhancedInputComp = Cast<UEnhancedInputComponent>(GetOwner()->InputComponent);

	InputData.InitialTransform = GetOwner()->GetActorTransform();
	SaveBindingsData();
}

void UP14InputRecordingComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	P14::Test::FJsonUtils::WriteInputData(GenerateFileName(), InputData);
	Super::EndPlay(EndPlayReason);
}

void UP14InputRecordingComponent::SaveBindingsData()
{
	FP14BindingsData BindingsData;
	BindingsData.WorldTime = GetWorld()->GetTimeSeconds();

	int32 Index = 0;
	for (const TUniquePtr<FEnhancedInputActionEventBinding>& Binding : EnhancedInputComp->GetActionEventBindings())
	{
		const UInputAction* Action = Binding->GetAction();
		BindingsData.ActionValues.Add({Action->GetFName(), EnhancedInputComp->GetBoundActionValue(Action).Get<FVector>()});

		const FString ActionString = Action->GetName();
		const FString ValueString  = EnhancedInputComp->GetBoundActionValue(Action).Get<FVector>().ToString();
		Index                      = ++Index % EnhancedInputComp->GetActionEventBindings().Num();
		GEngine->AddOnScreenDebugMessage(Index, 0.f, FColor::Cyan, FString::Format(L"Action {0}: {1}", {ActionString, ValueString}));
	}

	InputData.Bindings.Add(MoveTemp(BindingsData));
}

FString UP14InputRecordingComponent::GenerateFileName()
{
	return FPaths::Combine(FPaths::ProjectDir(), "Saved", "Data", "Project14", "CharacterInputData.json");
}
