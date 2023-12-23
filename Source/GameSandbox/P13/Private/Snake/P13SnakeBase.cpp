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

	AddSnakeElement(4, true);
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

void AP13SnakeBase::AddSnakeElement(const int32 ElementsNum, const bool bVisible)
{
	for (int Index = 0; Index < ElementsNum; ++Index)
	{
		const auto ElementLocation = FVector(SnakeElements.Num() * ElementSize, 0.f, 0.f);
		const auto ElementTransform = FTransform(GetActorLocation() - ElementLocation);

		AP13SnakeElementBase* SnakeElement = GetWorld()->SpawnActor<AP13SnakeElementBase>(SnakeElementClass, ElementTransform);
		if (!SnakeElement)
		{
			continue;
		}
		SnakeElement->GetMesh()->SetVisibility(bVisible);

		const int32 ElementIndex = SnakeElements.Add(SnakeElement);
		if (ElementIndex == 0)
		{
			SetHeadAppearance();
			SnakeElement->OnSnakeElementBeginOverlap.AddUObject(this, &ThisClass::OnSnakeElementOverlap);
		}
	}
}

void AP13SnakeBase::SnakeMove()
{
	/* First of all we need to know in which direction we are moving. */
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

	/* Snake body is moving from the tale to the head.
	 * At the end fo the loop SnakeElements[1] will be at the same location as the SnakeHead.
	 * To avoid colliding errors, we disable collision for the head while the snake is moving. */
	AP13SnakeElementBase* SnakeHead = SnakeElements[0];
	if (!SnakeHead)
	{
		return;
	}
	SnakeHead->ToggleCollision();
	
	/* By iterating through the snake elements starting from the tail (last element),
	 * we set the location of each element to the location of the previous element of the array.
	 * If the element was spawned invisible - after updating its location we restore visibility. */
	for (int32 Index = SnakeElements.Num() - 1; Index > 0; --Index)
	{
		AP13SnakeElementBase* CurrentElement = SnakeElements[Index];
		AP13SnakeElementBase const* PreviousElement = SnakeElements[Index - 1];

		if (!CurrentElement->GetMesh()->IsVisible())
		{
			CurrentElement->GetMesh()->SetVisibility(true);
		}
		const FVector PreviousLocation = PreviousElement->GetActorLocation();
		CurrentElement->SetActorLocation(PreviousLocation);
	}
	/* After then, we move head to the new calculated location. */
	SnakeHead->AddActorWorldOffset(MoveVector);
	
	/* Return the head ability to collide with objects after snake moving. */
	SnakeHead->ToggleCollision();
}

void AP13SnakeBase::OnSnakeElementOverlap(AP13SnakeElementBase* OverlappedElement, AActor* OverlappedWith)
{
	if (!OverlappedElement)
	{
		return;
	}

	const bool bFirst = SnakeElements.Find(OverlappedElement) == 0;
	if (IP13Respond* Responder = Cast<IP13Respond>(OverlappedWith))
	{
		Responder->Respond(this, bFirst);
		return;
	}
	Destroy();
}

void AP13SnakeBase::SetHeadAppearance()
{
	UMaterialInstanceDynamic* DynamicMaterial = SnakeElements[0]->GetMesh()->CreateAndSetMaterialInstanceDynamic(0);
	DynamicMaterial->SetVectorParameterValue(HeadColorName, HeadColor);
}
