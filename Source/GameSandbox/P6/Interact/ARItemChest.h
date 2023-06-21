// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P6/Interface/ARGameplayInterface.h"
#include "ARItemChest.generated.h"

UCLASS()
class GAMESANDBOX_API AARItemChest : public AActor, public IARGameplayInterface
{
	GENERATED_BODY()

public:
	AARItemChest();
	virtual void Tick(float DeltaTime) override;
	
	/* Interface */
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	virtual void OnActorLoaded_Implementation() override;
	
	/* Multiplayer */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnRep_LidOpened();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Mesh")
	UStaticMeshComponent* BaseMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Mesh")
	UStaticMeshComponent* LidMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Mesh")
	float TargetPitch = 110.f;
	
	UPROPERTY(ReplicatedUsing = "OnRep_LidOpened", BlueprintReadOnly, SaveGame)
	bool bLidOpen = false;
};
