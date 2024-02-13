// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P13SnakeBase.generated.h"

class AP13SnakeElementBase;

UENUM()
enum class EP13MoveDirection : uint8
{
	Up,
	Down,
	Left,
	Right
};

/* =================================== Class =============================== */
UCLASS()
class GAMESANDBOX_API AP13SnakeBase : public AActor
{
	GENERATED_BODY()

	/* ------------------------------- Super ------------------------------- */
public:
	AP13SnakeBase();
	virtual void Tick(const float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	/* ------------------------------- This -------------------------------- */
public:
	FORCEINLINE EP13MoveDirection GetMoveDirection() const { return LastMoveDirection; }
	void SetMoveDirection(const EP13MoveDirection NewDirection);
	void AddSnakeElement(const int32 ElementsNum = 1, const bool bVisible = false);

protected:
	void SnakeMove();
	void OnSnakeElementOverlap(AP13SnakeElementBase* OverlappedElement, AActor* OverlappedWith);

private:
	void SetHeadAppearance();

	/* ------------------------------ Members ------------------------------ */
protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++")
	TSubclassOf<AP13SnakeElementBase> SnakeElementClass;
	UPROPERTY(EditAnywhere, Category = "C++")
	float ElementSize = 100.f;
	UPROPERTY(EditAnywhere, Category = "C++")
	EP13MoveDirection LastMoveDirection = EP13MoveDirection::Up;
	UPROPERTY(EditAnywhere, Category = "C++")
	float SnakeSpeed = 1.f;

	UPROPERTY(EditAnywhere, Category = "C++")
	FName HeadColorName = NAME_None;
	UPROPERTY(EditAnywhere, Category = "C++")
	FLinearColor HeadColor = FLinearColor::Black;

private:
	UPROPERTY()
	TArray<AP13SnakeElementBase*> SnakeElements;
};
