// Copyright © 2024, Ivan Kinder

#include "Items/P14InventoryItem.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/P14InventoryComponent.h"
#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/Pawn.h"
#include "Kismet/KismetMathLibrary.h"

AP14InventoryItem::AP14InventoryItem()
{
	CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionSphereComponent");
	CollisionComponent->SetSphereRadius(Radius);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	CollisionComponent->SetGenerateOverlapEvents(true);
	SetRootComponent(CollisionComponent);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	MeshComponent->SetRelativeScale3D(FVector{0.5f});
	MeshComponent->SetCollisionProfileName("NoCollision");
	MeshComponent->SetupAttachment(CollisionComponent);

	TextComponent = CreateDefaultSubobject<UTextRenderComponent>("TextRendererComponent");
	TextComponent->AddLocalOffset({0.f, 0.f, Radius});
	TextComponent->SetupAttachment(CollisionComponent);
}

void AP14InventoryItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (MeshesMap.Contains(InventoryData.Type))
	{
		MeshComponent->SetStaticMesh(MeshesMap[InventoryData.Type]);
	}

	TextComponent->SetText(FText::AsNumber(InventoryData.Score));

	LazyInit();
	if (DynamicMaterial && MeshComponent)
	{
		const FLinearColor RandomColor = Colors[UKismetMathLibrary::RandomInteger(Colors.Num())];
		DynamicMaterial->SetVectorParameterValue("Color", RandomColor);
		MeshComponent->SetMaterial(0, DynamicMaterial);
	}
}

void AP14InventoryItem::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (Particles)
	{
		UNiagaraComponent* Particle = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Particles, GetActorLocation());
	}
}

void AP14InventoryItem::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	const APawn* Pawn = Cast<APawn>(OtherActor);
	if (!Pawn)
	{
		return;
	}

	UP14InventoryComponent* InventoryComponent = Pawn->FindComponentByClass<UP14InventoryComponent>();
	if (!InventoryComponent)
	{
		return;
	}

	if (!InventoryComponent->TryAddItem(InventoryData))
	{
		return;
	}

	Destroy();
}

void AP14InventoryItem::LazyInit()
{
	if (Material && !DynamicMaterial && MeshComponent->GetStaticMesh())
	{
		DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
	}

	if (Colors.IsEmpty())
	{
		Colors.Add(FLinearColor::Red);
		Colors.Add(FLinearColor::Yellow);
		Colors.Add(FLinearColor::Green);
		Colors.Add(FLinearColor::Blue);
	}
}
