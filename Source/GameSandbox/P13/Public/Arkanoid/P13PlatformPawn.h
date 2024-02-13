// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Pawn.h"
#include "P13PlatformPawn.generated.h"

class AP13Ball;
class UInputAction;
class UInputMappingContext;

UCLASS()
class GAMESANDBOX_API AP13PlatformPawn : public APawn
{
	GENERATED_BODY()

public:
	AP13PlatformPawn();
	virtual void Tick(const float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
	void OnSideInput(const FInputActionValue& Value);

private:
	void AddDefaultMappingContext();
	void CreateBall();
	void OnBallDeadHandle();

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ | Component")
	UStaticMeshComponent* PlatformMesh = nullptr;
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ | Component")
	USceneComponent* BallSpawnPoint = nullptr;
	UPROPERTY(EditAnywhere, Category = "C++ | Component")
	TSubclassOf<AP13Ball> BallClass = nullptr;

	UPROPERTY(EditAnywhere, Category = "C++ | Input")
	UInputMappingContext* DefaultContext = nullptr;
	UPROPERTY(EditAnywhere, Category = "C++ | Input")
	UInputAction* SideAction = nullptr;

	UPROPERTY(EditAnywhere, Category = "C++ | Param")
	float PlatformSpeed = 50.f;

private:
	TWeakObjectPtr<AP13Ball> CachedBall;
};
