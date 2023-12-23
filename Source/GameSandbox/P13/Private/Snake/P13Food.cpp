// Copyright © 2024, IKinder

#include "P13/Public/Snake/P13Food.h"

#include "P13/Public/Snake/P13SnakeBase.h"

AP13Food::AP13Food()
{

	FoodMesh = CreateDefaultSubobject<UStaticMeshComponent>("FoodStaticMeshComponent");
	FoodMesh->SetupAttachment(RootComponent);
	FoodMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	FoodMesh->SetCollisionResponseToAllChannels(ECR_Overlap);
}

void AP13Food::BeginPlay()
{
	Super::BeginPlay();
}

void AP13Food::Respond(AActor* Interactor, const bool bHead)
{
	if (Interactor && Interactor->IsA<AP13SnakeBase>() && bHead)
	{
		AP13SnakeBase* SnakeBase = StaticCast<AP13SnakeBase*>(Interactor);
		SnakeBase->AddSnakeElement();
		Destroy();
	}
}
