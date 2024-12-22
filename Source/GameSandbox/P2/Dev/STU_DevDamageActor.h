// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STU_DevDamageActor.generated.h"

UCLASS()
class GAMESANDBOX_API ASTU_DevDamageActor : public AActor
{
	GENERATED_BODY()

public:
	ASTU_DevDamageActor();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Kinder | Component")
	USceneComponent* SceneComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Component")
	float Radius = 200.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Component")
	int Segments = 24;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Component")
	FColor SphereColor = FColor::Red;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Component")
	float Damage = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Component")
	bool bDoFullDamage = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Component")
	TSubclassOf<UDamageType> DamageType;
};
