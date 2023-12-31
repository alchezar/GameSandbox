// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "P13/Public/Library/P13Types.h"
#include "P13InventoryComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FP13OnSwitchWeaponSignature, FName /*WeaponID*/, const FP13WeaponDynamicInfo* /*Info*/)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMESANDBOX_API UP13InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                                Super                                  *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
public:
	UP13InventoryComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                                 This                                  *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
public:
	FORCEINLINE int32 GetWeaponsCount() const { return WeaponSlots.Num(); }
	int32 GetWeaponSlotIndex(const FName WeaponID) const;
	FName GetWeaponIdBySlotIndex(const int32 Index = 0) const;
	FP13WeaponDynamicInfo GetWeaponDynamicInfo(const int32 Index);
	
	void SetWeaponInfo(const int32 WeaponIndex, const FP13WeaponDynamicInfo NewInfo);
	
	bool TrySwitchWeaponToIndex(const int32 NewIndex, int32 OldIndex, FP13WeaponDynamicInfo OldInfo);
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                               Variables                               *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
public:
	FP13OnSwitchWeaponSignature OnSwitchWeapon;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Weapon")
	TArray<FP13WeaponSlot> WeaponSlots;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Weapon")
	TArray<FP13AmmoSlot> AmmoSlots;
};
