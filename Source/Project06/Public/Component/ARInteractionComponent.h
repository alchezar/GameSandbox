// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ARInteractionComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT06_API UARInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UARInteractionComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void PrimaryInteract(APawn* InstigatorPawn);

protected:
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable)
	void ServerInteract(APawn* InstigatorPawn);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Interaction")
	float SearchDistance = 1000.f;
};
