// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "P17CharacterBase.generated.h"

class UP17Data_StartupBase;
class UP17AttributeSet;
class UP17AbilitySystemComponent;

UCLASS()
class PROJECT17_API AP17CharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
public:
	AP17CharacterBase();

	/// @par AActor interface
protected:
	virtual void BeginPlay() override;

	/// @par APawn interface
public:
	virtual void PossessedBy(AController* NewController) override;

	/// @par IAbilitySystemInterface
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
	_NODISCARD
	UP17AbilitySystemComponent* GetProjectAbilitySystemComponent() const { return AbilitySystemComponent; };
	_NODISCARD
	UP17AttributeSet* GetAttributeSet() const { return AttributeSet; };

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "C++ | Component")
	TObjectPtr<UP17AbilitySystemComponent> AbilitySystemComponent = nullptr;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "C++ | Component")
	TObjectPtr<UP17AttributeSet> AttributeSet = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Data")
	TSoftObjectPtr<UP17Data_StartupBase> StartupData = nullptr;
};
