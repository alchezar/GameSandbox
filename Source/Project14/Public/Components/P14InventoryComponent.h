// Copyright © 2024, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Utils/P14Types.h"
#include "P14InventoryComponent.generated.h"

UCLASS(ClassGroup=(P14), meta=(BlueprintSpawnableComponent))
class PROJECT14_API UP14InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
public:
	UP14InventoryComponent();

protected:
	virtual void BeginPlay() override;

	/* ------------------------------- This -------------------------------- */
public:
	bool TryAddItem(const FP14InventoryData& InData);
	UFUNCTION(BlueprintCallable, Category = "C++")
	int32 GetInventoryAmountByType(const EP14InventoryItemType InType) const;

	/* ------------------------------ Fields ------------------------------- */
public:
	UPROPERTY(EditAnywhere, BlueprintAssignable, Category = "C++")
	FP14OnScoreUpdated OnScoreUpdated;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++")
	TMap<EP14InventoryItemType, int32> InventoryLimits;

private:
	TMap<EP14InventoryItemType, int32> Inventory;
};
