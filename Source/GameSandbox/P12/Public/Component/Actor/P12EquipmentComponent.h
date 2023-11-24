// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "P12/Public/Util/P12CoreTypes.h"
#include "P12EquipmentComponent.generated.h"

class UP12WeaponWheelWidget;
class UP12EquipmentViewWidget;
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

	bool AddEquipmentItemToSlot(const TSubclassOf<AP12EquipableItem>& EquipableItemClass, int32 SlotIndex);
	void RemoveItemFromSlot(const int32 SlotIndex);
	void OpenViewEquipment(APlayerController* InPlayerController);
	void CloseViewEquipment();
	bool GetIsViewVisible() const;
	FORCEINLINE const TArray<AP12EquipableItem*>& GetItems() const { return ItemsArray; };
	void OpenWeaponWheel(APlayerController* InPlayerController);
	bool GetIsSelectingWeapon() const;
	void ConfirmWeaponSelection();

protected:
	virtual void BeginPlay() override;

	void CreateEquipmentWidgets(APlayerController* InPlayerController);

private:
	void CreateLoadout();
	void AutoEquip();

	UFUNCTION(Server, Reliable)
	void Server_EquipItemInSlot(EP12EquipmentSlot Slot);
	UFUNCTION()
	void OnRep_CurrentEquippedSlot(EP12EquipmentSlot OldCurrentSlot);
	UFUNCTION()
	void OnRep_ItemsArray();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Loadout")
	TMap<EP12AmmunitionType, int32> MaxAmmunitionAmount;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Loadout")
	TMap<EP12EquipmentSlot, TSubclassOf<AP12EquipableItem>> ItemsLoadout;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Loadout")
	TSet<EP12EquipmentSlot> IgnoredSlotsWhileSwitching;
	UPROPERTY(EditAnywhere, Category = "C++ | Loadout")
	EP12EquipmentSlot AutoEquipItemInSlot = EP12EquipmentSlot::None;

	UPROPERTY(EditAnywhere, Category = "C++ | View")
	TSubclassOf<UP12EquipmentViewWidget> EquipmentViewWidgetClass;
	UPROPERTY(EditAnywhere, Category = "C++ | View")
	TSubclassOf<UP12WeaponWheelWidget> WeaponWheelWidgetClass;
	
private:
	TWeakObjectPtr<AP12BaseCharacter> CachedCharacter;
	UPROPERTY(ReplicatedUsing = "OnRep_CurrentEquippedSlot")
	EP12EquipmentSlot CurrentEquippedSlot = EP12EquipmentSlot::None;
	EP12EquipmentSlot PreviousEquippedSlot = EP12EquipmentSlot::None;
	
	UPROPERTY(Replicated)
	TArray<int32> AmmunitionArray;
	UPROPERTY(ReplicatedUsing = "OnRep_ItemsArray")
	TArray<AP12EquipableItem*> ItemsArray;
	
	UPROPERTY()
	AP12EquipableItem* CurrentEquippedItem = nullptr;
	UPROPERTY()
	AP12RangeWeaponItem* CurrentEquippedWeapon = nullptr;
	UPROPERTY()
	AP12ThrowableItem* CurrentThrowableItem = nullptr;
	UPROPERTY()
	AP12MeleeWeaponItem* CurrentMeleeWeapon = nullptr;

	UPROPERTY()
	UP12EquipmentViewWidget* EquipmentViewWidget = nullptr;
	UPROPERTY()
	UP12WeaponWheelWidget* WeaponWheelWidget = nullptr;
};
