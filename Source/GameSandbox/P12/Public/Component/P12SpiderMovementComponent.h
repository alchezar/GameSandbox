// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "P12SpiderMovementComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMESANDBOX_API UP12SpiderMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()

public:
	UP12SpiderMovementComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual bool IsFalling() const override;
	FORCEINLINE bool GetIsFalling() const { return bFalling; }
	void JumpStart();

protected:
	virtual void BeginPlay() override;

private:
	void MovePawn(const float DeltaTime);

protected:
	UPROPERTY(EditAnywhere, Category = "C++")
	float MaxSpeed = 500.f;
	UPROPERTY(EditAnywhere, Category = "C++")
	float InitJumpVelocity = 500.f;

private:
	bool bFalling = false;
	FVector VerticalVelocity = FVector::ZeroVector;
};
