// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "ARAbilityComponent.generated.h"

class UARAbility;
class UARAbilityComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAROnAbilityChangedSignature, UARAbilityComponent*, AbilityComp, UARAbility*, Ability);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMESANDBOX_API UARAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UARAbilityComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable, Category = "C++ | Action")
	void AddAbility(AActor* Instigator, TSubclassOf<UARAbility> ActionClass);
	UFUNCTION(BlueprintCallable, Category = "C++ | Action")
	void RemoveAbility(UARAbility* AbilityToRemove);
	UFUNCTION(BlueprintCallable, Category = "C++ | Action")
	bool StartAbilityByName(AActor* Instigator, const FName AbilityName);
	UFUNCTION(BlueprintCallable, Category = "C++ | Action")
	bool StopAbilityByName(AActor* Instigator, const FName AbilityName);

	void HandleActionStart(const FGameplayTagContainer& GrantsTags, UARAbility* Ability);
	void HandleActionStop(const FGameplayTagContainer& GrantsTags, UARAbility* Ability);

	TArray<UARAbility*> GetAbilities() const;

	/* Multiplayer */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable)
	void ServerStartAbility(AActor* Instigator, const FName AbilityName);
	UFUNCTION(Server, Reliable)
	void ServerStopAbility(AActor* Instigator, const FName AbilityName);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Tag")
	FGameplayTagContainer ActiveGameplayTags;

	FAROnAbilityChangedSignature OnAbilityStarted;
	FAROnAbilityChangedSignature OnAbilityStopped;

protected:
	UPROPERTY(Replicated)
	TArray<UARAbility*> Abilities;
};
