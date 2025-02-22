// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "ARAbility.generated.h"

class UARAbilityComponent;
class UWorld;

USTRUCT()
struct FAbilityRepData
{
	GENERATED_BODY()

	FAbilityRepData() : bRunning(false), Instigator(nullptr) {}
	
	FAbilityRepData(const bool bNewRunning, AActor* NewInstigator)
		: bRunning(bNewRunning), Instigator(NewInstigator) {}

	UPROPERTY()
	bool bRunning;
	UPROPERTY()
	AActor* Instigator;
};

UCLASS(Blueprintable)
class PROJECT06_API UARAbility : public UObject
{
	GENERATED_BODY()

public:
	virtual UWorld* GetWorld() const override;
	
	void Initialize(UARAbilityComponent* NewAbilityComp);
	UFUNCTION(BlueprintCallable, Category = "C++ | Action")
	bool GetIsRunning() const;	
	UFUNCTION(BlueprintNativeEvent, Category = "C++ | Action")
	void StartAbility(AActor* Instigator);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "C++ | Action")
	void StopAbility(AActor* Instigator);
	UFUNCTION(BlueprintNativeEvent, Category = "C++ | Action")
	bool CanStart(AActor* Instigator);

	UTexture2D* GetIcon() const;	
	UFUNCTION(BlueprintCallable, Category = "C++ | Action")
	UARAbilityComponent* GetAbilityComponent() const;

	/* Multiplayer */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;
	UFUNCTION()
	void OnRep_RepData();

public:
	UPROPERTY(Replicated)
	UARAbilityComponent* AbilityComponent;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Action")
	FName AbilityName;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Action")
	bool bAutostart = false;

	/* Multiplayer */
	UPROPERTY(ReplicatedUsing = OnRep_RepData)
	FAbilityRepData RepData;
	// bool bRunning;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Tag")
	FGameplayTagContainer GrantsTags;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Tag")
	FGameplayTagContainer BlockedTags;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | UI")
	TSoftObjectPtr<UTexture2D> Icon;

};
