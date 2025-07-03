// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Component/P17ExtensionBaseComponent.h"
#include "Util/P17GameplayTags.h"
#include "P17CombatPawnComponent.generated.h"

class AP17WeaponBase;

UENUM(BlueprintType)
enum class EP17ToggleDamageType : uint8
{
	EquippedWeapon,
	LeftHand,
	RightHand
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT17_API UP17CombatPawnComponent : public UP17ExtensionBaseComponent
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
	UFUNCTION(BlueprintCallable, Category = "C++ | Combat")
	void RegisterSpawnedWeapon(const FGameplayTag InWeaponTag, AP17WeaponBase* InWeapon, const bool bRegisterAsEquipped = false);
	UFUNCTION(BlueprintCallable, Category = "C++ | Combat")
	AP17WeaponBase* GetCarriedWeaponBy(const FGameplayTag InWeaponTag) const;
	UFUNCTION(BlueprintCallable, Category = "C++ | Combat")
	AP17WeaponBase* GetEquippedWeapon() const;

	UFUNCTION(BlueprintCallable, Category = "C++ | Weapon")
	void SetEquippedWeaponTag(const FGameplayTag InWeaponTag);
	UFUNCTION(BlueprintCallable, Category = "C++ | Weapon")
	void ToggleWeaponCollision(const bool bEnable, const EP17ToggleDamageType InToggleDamageType = EP17ToggleDamageType::EquippedWeapon);

protected:
	virtual void OnHitTargetActorCallback(AActor* HitActor);
	virtual void OnWeaponPulledFromActorCallback(AActor* PulledActor);

	void SendGameplayEventToActor(AActor* HitActor, const FGameplayTag EventTag = P17::Tags::Shared_Event_Hit_Melee) const;

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++ | Combat")
	FGameplayTag EquippedWeaponTag;

protected:
	UPROPERTY()
	TArray<AActor*> OverlappedActors = {};

private:
	UPROPERTY()
	TMap<FGameplayTag, AP17WeaponBase*> CarriedWeaponMap = {};
};
