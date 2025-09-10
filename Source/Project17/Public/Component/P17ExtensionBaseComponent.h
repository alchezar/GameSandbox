// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "Project17.h"
#include "Components/ActorComponent.h"
#include "P17ExtensionBaseComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT17_API UP17ExtensionBaseComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	template <typename T>
	T* GetOwningPawn() const
	{
		static_assert(TPointerIsConvertibleFromTo<T, APawn>::Value, "<T> must be derived from APawn!");

		return Cast<T>(GetOwner());
	}

	APawn* GetOwningPawn() const
	{
		return GetOwningPawn<APawn>();
	}

	template <typename T>
	T* GetOwningController() const
	{
		static_assert(TPointerIsConvertibleFromTo<T, AController>::Value, "<T> must be derived from AController!");

		const APawn* Pawn = GetOwningPawn();
		RETURN_IF(!Pawn, nullptr);

		return Pawn->GetController<T>();
	}
};
