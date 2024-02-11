// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "P13/Public/Intearface/P13PickupInterface.h"
#include "P13/Public/Library/P13Types.h"
#include "P13InventoryComponent.generated.h"

class AP13PickingUpBase;
class UP13GameInstance;
class AP13Weapon;

DECLARE_MULTICAST_DELEGATE_TwoParams(FP13OnCurrentWeaponUpdatedSignature, const FP13WeaponSlot& /*NewWeaponSLot*/, const int32 /*CurrentIndex*/)
DECLARE_MULTICAST_DELEGATE_TwoParams(FP13OnSwitchWeaponSignature, const int32 /*NewWeaponIndex*/, AP13Weapon* /*NewWeapon*/)
DECLARE_MULTICAST_DELEGATE_ThreeParams(FP13OnAmmoChangedSignature, EP13AmmoType /*AmmoType*/, int32 /*NewWeaponAmmoCount*/, int32 /*NewInventoryAmmoCount*/)
DECLARE_MULTICAST_DELEGATE(FP13OnInventoryUpdatedSingature);
DECLARE_MULTICAST_DELEGATE_TwoParams(FP13OnNewWeaponTakenSignature, const int32 /*NewWeaponIndex*/, const FP13WeaponSlot& /*NewWeaponSLot*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FP13OnNewAmmoTakenSignature, const FP13AmmoSlot& /*NewAmmoSlot*/)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMESANDBOX_API UP13InventoryComponent : public UActorComponent, public IP13PickupInterface
{
	GENERATED_BODY()

	/* ------------------------------- Super ------------------------------- */
public:
	UP13InventoryComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* ----------------------------- Interface ----------------------------- */
public:
	virtual void PickupAmmoToInventory(const FP13AmmoSlot& NewAmmoSlot, AP13PickingUpBase* PickupActor) override;
	virtual void PickupWeaponToInventory(const FP13AmmoSlot& NewAmmoSlot, const FP13WeaponSlot& NewWeaponSlot, AP13PickingUpBase* PickupActor) override;

	/* ------------------------------- This -------------------------------- */
public:
	FORCEINLINE int32 GetWeaponSlotsCount() const { return WeaponSlots.Num(); }
	FORCEINLINE int32 GetAmmoSlotsCount() const { return AmmoSlots.Num(); }
	FORCEINLINE int32 GetCurrentWeaponIndex() const { return CurrentWeaponIndex; }
	FORCEINLINE FP13WeaponSlot GetWeaponSlot(const int32 Index) const { return WeaponSlots[Index]; }
	FORCEINLINE FP13AmmoSlot GetAmmoSlot(const int32 Index) const { return AmmoSlots[Index]; }
	FORCEINLINE TArray<FP13WeaponSlot> GetAllWeaponSlots() const { return WeaponSlots; }
	FORCEINLINE TArray<FP13AmmoSlot> GetAllAmmoSlots() const { return AmmoSlots; }
	FName GetWeaponIdBySlotIndex(const int32 Index = 0) const;
	FP13WeaponDynamicInfo GetWeaponDynamicInfo(const int32 Index);
	void SetWeaponInfo(const FP13WeaponDynamicInfo NewInfo, const bool bIncrease = false);
	bool TrySwitchWeaponToIndex(const int32 NewIndex, const int32 OldIndex, FP13WeaponDynamicInfo OldInfo, const bool bNext = true);
	int32 FindMaxAvailableRound(const int32 OldRoundNum, const int32 MaxRound);
	bool TryDropCurrentWeapon(const AP13Weapon* CurrentWeapon, bool bTakeNext = true);
	void ClearWeaponSlots();
	void SortAmmoSlots();
	void RefreshSlots();

private:
	void CacheGameInstance();
	bool TryUpdateSlotsFromData();
	bool TryLoadSlotsFromPlayerState();
	void TakingAmmoToInventoryAttempt(const FP13AmmoSlot& NewAmmoSlot, AP13PickingUpBase* PickupActor);
	void TakingWeaponToInventoryAttempt(const FP13AmmoSlot& NewAmmoSlot, const FP13WeaponSlot& NewWeaponSlot, AP13PickingUpBase* PickupActor);
	bool TryTakeWeaponToInventory(const FP13WeaponSlot& NewWeaponSlot);
	bool TryTakeAmmoToInventory(const FP13AmmoSlot& NewAmmoSlot);
	
	/* ------------------------------ Network ------------------------------ */
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	UFUNCTION(Server, Reliable)
	void Server_RefreshSlots();
	UFUNCTION(Server, Reliable)
	void Server_SpawnDroppedWeapon(const FTransform& SpawnTransform, const FP13WeaponDrop DropWeaponInfo);
	
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void Multicast_OnCurrentWeaponUpdatedBroadcast(const FP13WeaponSlot& NewWeaponSlot, const int32 CurrentIndex);
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void Multicast_OnSwitchWeaponBroadcast(const int32 NewWeaponIndex, AP13Weapon* NewWeapon);
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void Multicast_OnAmmoChangedBroadcast(EP13AmmoType AmmoType, int32 NewWeaponAmmoCount, int32 NewInventoryAmmoCount);
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void Multicast_OnInventoryUpdatedBroadcast();
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void Multicast_OnNewWeaponTakenBroadcast(const int32 NewWeaponIndex, const FP13WeaponSlot& NewWeaponSlot);
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void Multicast_OnNewAmmoTakenBroadcast(const FP13AmmoSlot& NewAmmoSlot);
	
	UFUNCTION(Server, Reliable)
	void Server_TakingAmmoToInventoryAttempt(const FP13AmmoSlot& NewAmmoSlot, AP13PickingUpBase* PickupActor);
	UFUNCTION(Server, Reliable)
	void Server_TakingWeaponToInventoryAttempt(const FP13AmmoSlot& NewAmmoSlot, const FP13WeaponSlot& NewWeaponSlot, AP13PickingUpBase* PickupActor);

	/* ----------------------------- Variables ----------------------------- */
public:
	FP13OnCurrentWeaponUpdatedSignature OnCurrentWeaponUpdated;
	FP13OnSwitchWeaponSignature OnSwitchWeapon;
	FP13OnAmmoChangedSignature OnAmmoChanged;
	FP13OnInventoryUpdatedSingature OnInventoryUpdated;
	FP13OnNewWeaponTakenSignature OnNewWeaponTaken;
	FP13OnNewAmmoTakenSignature OnNewAmmoTaken;

protected:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "C++ | Weapon")
	TArray<FP13WeaponSlot> WeaponSlots;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "C++ | Weapon")
	TArray<FP13AmmoSlot> AmmoSlots;

private:
	int32 AmmoLeft = 0;
	TSoftObjectPtr<UP13GameInstance> GameInstanceCached;
	int32 CurrentWeaponIndex = 0;
};
