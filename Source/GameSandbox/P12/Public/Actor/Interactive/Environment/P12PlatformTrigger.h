// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P12PlatformTrigger.generated.h"

class UBoxComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FP12OnTriggerActivatedSignature, bool /*bActive*/)

USTRUCT(BlueprintType)
struct FP12PlatformAppearance
{
	GENERATED_BODY()

	FP12PlatformAppearance() {}

	FP12PlatformAppearance(const FLinearColor NewColor, const FLinearColor NewEmission)
		: Color(NewColor), Emission(NewEmission) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor Color = FLinearColor::Black;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor Emission = FLinearColor::Black;
};

UCLASS()
class GAMESANDBOX_API AP12PlatformTrigger : public AActor
{
	GENERATED_BODY()

public:
	AP12PlatformTrigger();
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnTriggerBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnTriggerEndOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex);

private:
	void SetIsActivated(const bool bNewActive);
	void SaveInactiveAppearance(const UMaterialInstanceDynamic* CurrentDynamicMaterial);

	UFUNCTION()
	void OnRep_IsActivated(bool bWasActivated);
	
public:
	FP12OnTriggerActivatedSignature OnTriggerActivated;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++")
	UStaticMeshComponent* TriggerBase;
	UPROPERTY(EditDefaultsOnly, Category = "C++")
	UBoxComponent* PlatformTrigger;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FP12PlatformAppearance ActiveAppearance;

private:
	UPROPERTY(ReplicatedUsing = "OnRep_IsActivated")
	bool bActive = false;
	UPROPERTY()
	TArray<APawn*> OverlappedPawns;
	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial;
	FP12PlatformAppearance InactiveAppearance;
};
