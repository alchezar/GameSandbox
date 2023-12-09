// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P12StreamingSubsystemVolume.generated.h"

class UP12StreamingSubsystem;
class UBoxComponent;

UCLASS()
class GAMESANDBOX_API AP12StreamingSubsystemVolume : public AActor
{
	GENERATED_BODY()

public:
	AP12StreamingSubsystemVolume();
	FORCEINLINE const TSet<FString>& GetLevelsToLoad() const { return LevelsToLoad; }
	FORCEINLINE const TSet<FString>& GetLevelsToUnload() const { return LevelsToUnload; }
	void HandleCharacterOverlapBegin(ACharacter* Character);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	UFUNCTION()
	void OnCollisionBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnCollisionEndOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	UBoxComponent* Collision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TSet<FString> LevelsToLoad;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TSet<FString> LevelsToUnload;

private:
	TWeakObjectPtr<UP12StreamingSubsystem> StreamingSubsystem;
	TWeakObjectPtr<ACharacter> OverlappedCharacter;
};
