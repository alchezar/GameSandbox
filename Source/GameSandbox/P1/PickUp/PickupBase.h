// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupBase.generated.h"

UCLASS()
class GAMESANDBOX_API APickUpBase : public AActor
{
	GENERATED_BODY()

public:
	APickUpBase();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure, Category = "Kinder")
	bool GetPickUpActive() const;
	UFUNCTION(BlueprintCallable, Category = "Kinder")
	void SetPickUpActive(const bool NewActive);

	UFUNCTION(BlueprintNativeEvent)
	void OnPickupCollect();

protected:
	virtual void BeginPlay() override;

#pragma region Component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder")
	UStaticMeshComponent* PickupMesh;
#pragma  endregion

private:
	bool bActive = true;
};
