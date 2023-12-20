// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P13Ball.generated.h"

class AP13PlatformPawn;
DECLARE_MULTICAST_DELEGATE(FP12OnBallDeadSignature)

UCLASS()
class GAMESANDBOX_API AP13Ball : public AActor
{
	GENERATED_BODY()

public:
	AP13Ball();
	virtual void Tick(const float DeltaTime) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	void AddDelegateHandle(const FDelegateHandle InDeadDelegateHandle);

protected:
	virtual void BeginPlay() override;

private:
	void MoveBall(const float DeltaTime);
	void TryToDestroyBall(const AActor* OtherActor);

public:
	FP12OnBallDeadSignature OnBallDead;

protected:
	UPROPERTY(EditAnywhere, Category = "C++")
	UStaticMeshComponent* RootMesh = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "C++")
	float Speed = 10.f;
	UPROPERTY(EditAnywhere, Category = "C++")
	FVector Direction = FVector(100.f, 0.f, 0.f);

private:
	FDelegateHandle DeadDelegateHandle;
};
