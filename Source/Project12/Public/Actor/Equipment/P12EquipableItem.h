// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Util/P12CoreTypes.h"
#include "P12EquipableItem.generated.h"

class UP12ReticleWidget;
class AP12BaseCharacter;
class UP12EquipmentComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FP12OnEquipmentStateChangedSignature, bool);

UCLASS(Abstract, NotBlueprintable)
class PROJECT12_API AP12EquipableItem : public AActor
{
	GENERATED_BODY()

public:
	AP12EquipableItem();
	virtual void SetOwner(AActor* NewOwner) override;
	EP12EquipableItemType GetItemType() const { return ItemType; };
	FORCEINLINE FName GetUnEquippedSocketName() const { return UnEquippedSocketName; }
	FORCEINLINE FName GetEquippedSocketName() const { return EquippedSocketName; }
	FORCEINLINE UAnimMontage* GetCharacterEquipAnimMontage() const { return CharacterEquipAnimMontage; }
	FORCEINLINE TSoftObjectPtr<UP12EquipmentComponent> GetCachedEquipment() const { return CachedEquipment; }
	FORCEINLINE TWeakObjectPtr<AP12BaseCharacter> GetCachedCharacter() const { return CachedCharacter; }
	FORCEINLINE FName GetDataTableID() const { return DataTableID; }
	void CacheEquipmentComponent(UP12EquipmentComponent* EquipmentComp);
	virtual void AttachItem(const FName AttachSocketName);

	void Equip();
	void Unequip();
	UFUNCTION(Client, Reliable)
	void Client_ToggleReticle(const bool bShow) const;

	bool GetIsSlotCompatible(EP12EquipmentSlot Slot) const;

protected:
	virtual void InitAnimNotify();
	virtual void BeginPlay() override;
	virtual void OnEquippingStartedHandle(USkeletalMeshComponent* SkeletalMeshComponent);
	virtual void OnEquippingFinishedHandle(USkeletalMeshComponent* SkeletalMeshComponent);

public:
	FP12OnEquipmentStateChangedSignature OnEquipmentStateChanged;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++")
	EP12EquipableItemType ItemType = EP12EquipableItemType::None;
	UPROPERTY(EditDefaultsOnly, Category = "C++")
	FName UnEquippedSocketName = NAME_None;
	UPROPERTY(EditDefaultsOnly, Category = "C++")
	FName EquippedSocketName = NAME_None;

	UPROPERTY(EditDefaultsOnly, Category = "C++")
	TArray<EP12EquipmentSlot> CompatibleEquipmentSlots;

	UPROPERTY(EditDefaultsOnly, Category = "C++ | Animation | Character")
	UAnimMontage* CharacterEquipAnimMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "C++ | Reticle")
	EP12ReticleType ReticleType = EP12ReticleType::Default;

	UPROPERTY(EditDefaultsOnly, Category = "C++")
	FName DataTableID = NAME_None;

private:
	TSoftObjectPtr<UP12EquipmentComponent> CachedEquipment;
	TWeakObjectPtr<AP12BaseCharacter> CachedCharacter;
};
