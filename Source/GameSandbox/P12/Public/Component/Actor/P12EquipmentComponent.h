// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "P12/Public/Util/P12CoreTypes.h"
#include "P12EquipmentComponent.generated.h"

class AP12MeleeWeaponItem;
class AP12ThrowableItem;
class AP12EquipableItem;
class AP12BaseCharacter;
class AP12RangeWeaponItem;

typedef TArray<int32, TInlineAllocator<static_cast<uint32>(EP12AmmunitionType::MAX)>> TP12AmmunitionArray;
typedef TArray<AP12EquipableItem*, TInlineAllocator<static_cast<uint32>(EP12EquipmentSlot::MAX)>> TP12ItemsArray;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMESANDBOX_API UP12EquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UP12EquipmentComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	FORCEINLINE AP12RangeWeaponItem* GetCurrentEquippedWeapon() const { return CurrentEquippedWeapon; }
	FORCEINLINE AP12MeleeWeaponItem* GetCurrentMeleeWeapon() const { return CurrentMeleeWeapon; }
	EP12EquipableItemType GetCurrentEquippedItemType() const;
	int32 GetMaxAvailableAmmoAmount(const EP12AmmunitionType AmmoType);
	void DecreaseMaxAvailableAmmoAmount(const EP12AmmunitionType AmmoType, const int32 AmmoToDecrease);
	
	void EquipItemInSlot(EP12EquipmentSlot Slot);
	void EquipNextItem();
	void EquipPreviousItem();
	void UnEquipCurrentItem();
	void EquipCurrentItem();

	void TakeCurrentThrowableItem();
	void LaunchCurrentThrowableItem();

protected:
	virtual void BeginPlay() override;

private:
	void CreateLoadout();
	void AutoEquip();

	UFUNCTION(Server, Reliable)
	void Server_EquipItemInSlot(EP12EquipmentSlot Slot);
	UFUNCTION()
	void OnRep_CurrentEquippedSlot(EP12EquipmentSlot OldCurrentSlot);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Loadout")
	TMap<EP12AmmunitionType, int32> MaxAmmunitionAmount;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Loadout")
	TMap<EP12EquipmentSlot, TSubclassOf<AP12EquipableItem>> ItemsLoadout;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Loadout")
	TSet<EP12EquipmentSlot> IgnoredSlotsWhileSwitching;
	UPROPERTY(EditAnywhere, Category = "C++ | Loadout")
	EP12EquipmentSlot AutoEquipItemInSlot = EP12EquipmentSlot::None;
	
private:
	TWeakObjectPtr<AP12BaseCharacter> CachedCharacter;
	UPROPERTY(ReplicatedUsing = "OnRep_CurrentEquippedSlot")
	EP12EquipmentSlot CurrentEquippedSlot = EP12EquipmentSlot::None;
	EP12EquipmentSlot PreviousEquippedSlot = EP12EquipmentSlot::None;
	TP12AmmunitionArray AmmunitionArray;
	TP12ItemsArray ItemsArray;
	UPROPERTY()
	AP12EquipableItem* CurrentEquippedItem = nullptr;
	UPROPERTY()
	AP12RangeWeaponItem* CurrentEquippedWeapon = nullptr;
	UPROPERTY()
	AP12ThrowableItem* CurrentThrowableItem = nullptr;
	UPROPERTY()
	AP12MeleeWeaponItem* CurrentMeleeWeapon = nullptr;
};
