// Copyright © 2024, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Utils/P14Types.h"
#include "P14InventoryItem.generated.h"

class UNiagaraSystem;
class UTextRenderComponent;
class USphereComponent;

UCLASS(Abstract)
class PROJECT14_API AP14InventoryItem : public AActor
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
public:
	AP14InventoryItem();
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/* ------------------------------- This -------------------------------- */
public:
	_NODISCARD float                                                GetRadius() const { return Radius; }
	_NODISCARD FP14InventoryData                                    GetInventoryData() const { return InventoryData; };
	_NODISCARD TMap<EP14InventoryItemType, TObjectPtr<UStaticMesh>> GetMeshesMap() const { return MeshesMap; };

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:
	UFUNCTION(BlueprintCallable)
	void InitItem(const FLinearColor InColor);

private:
	void LazyInit();

	/* ------------------------------ Fields ------------------------------- */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++ | Components")
	TObjectPtr<USphereComponent> CollisionComponent = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++ | Components")
	TObjectPtr<UStaticMeshComponent> MeshComponent = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++ | Components")
	TObjectPtr<UTextRenderComponent> TextComponent = nullptr;
	UPROPERTY(EditAnywhere, Category = "C++")
	TObjectPtr<UNiagaraSystem> Particles = nullptr;
	UPROPERTY(EditAnywhere, Category = "C++")
	TObjectPtr<UMaterialInterface> Material = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	float Radius = 30.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FP14InventoryData InventoryData = {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TMap<EP14InventoryItemType, TObjectPtr<UStaticMesh>> MeshesMap = {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TArray<FLinearColor> Colors = {};

private:
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynamicMaterial = nullptr;
};
