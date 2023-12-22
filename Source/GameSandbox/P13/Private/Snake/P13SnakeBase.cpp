// Copyright © 2024, IKinder

#include "P13/Public/Snake/P13SnakeBase.h"

#include "P13/Public/Snake/P13SnakeElementBase.h"

AP13SnakeBase::AP13SnakeBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AP13SnakeBase::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickInterval(SnakeSpeed);

	AddSnakeElement(4);
}

void AP13SnakeBase::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
	SnakeMove();
}

void AP13SnakeBase::SetMoveDirection(const EP13MoveDirection NewDirection)
{
	LastMoveDirection = NewDirection;
}

void AP13SnakeBase::AddSnakeElement(const int32 ElementsNum)
{
	for (int Index = 0; Index < ElementsNum; ++Index)
	{
		const auto ElementLocation  = FVector(SnakeElements.Num() * ElementSize, 0.f, 0.f);
		const auto ElementTransform = FTransform(GetActorLocation() - ElementLocation);
		AP13SnakeElementBase* SnakeElement = GetWorld()->SpawnActor<AP13SnakeElementBase>(SnakeElementClass, ElementTransform);
		const int32 ElementIndex = SnakeElements.Add(SnakeElement);
		
		if (ElementIndex == 0)
		{
			SetHeadAppearance();
		}
	}
}

void AP13SnakeBase::SnakeMove()
{
	FVector MoveVector = FVector::ZeroVector;
	const float MoveSpeed = ElementSize;
	
	switch (LastMoveDirection)
	{
	case EP13MoveDirection::Up:
		MoveVector.X += MoveSpeed;
		break;
	case EP13MoveDirection::Down:
		MoveVector.X -= MoveSpeed;
		break;
	case EP13MoveDirection::Left:
		MoveVector.Y -= MoveSpeed;
		break;
	default:
		MoveVector.Y += MoveSpeed;
	}

	for (int32 Index = SnakeElements.Num() - 1; Index > 0; --Index)
	{
		AP13SnakeElementBase* CurrentElement = SnakeElements[Index];
		AP13SnakeElementBase const* PreviousElement = SnakeElements[Index - 1];
		
		const FVector PreviousLocation = PreviousElement->GetActorLocation();
		CurrentElement->SetActorLocation(PreviousLocation);
	}
	SnakeElements[0]->AddActorWorldOffset(MoveVector);
}

void AP13SnakeBase::SetHeadAppearance()
{
	UMaterialInstanceDynamic* DynamicMaterial = SnakeElements[0]->GetMesh()->CreateAndSetMaterialInstanceDynamic(0);
	DynamicMaterial->SetVectorParameterValue(HeadColorName, HeadColor);
}
