// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ARAICharacter.generated.h"

class UARDamagePopUp;
class UARAbilityComponent;
class UARWorldUserWidget;
class UPawnSensingComponent;
class UARAttributesComponent;

UCLASS()
class GAMESANDBOX_API AARAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AARAICharacter();
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;

	bool IsAlive() const;
	void SetTeamColor(const FLinearColor& Color);

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);
	UFUNCTION()
	void OnHealthChangedHandle(AActor* InstigatorActor, UARAttributesComponent* OwningComp, float NewHealth, float Delta);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPawnSeen(APawn* Pawn);

private:
	AActor* GetTargetActor() const;
	void SetTargetActor(AActor* NewTarget);

protected:
	UPROPERTY(EditAnywhere, Category = "C++ | Component")
	UPawnSensingComponent* PawnSensingComp;
	UPROPERTY(EditAnywhere, Category = "C++ | Component")
	UARAttributesComponent* AttributesComp;
	UPROPERTY(EditAnywhere, Category = "C++ | Component")
	UARAbilityComponent* AbilityComp;
	UPROPERTY(EditAnywhere, Category = "C++ | UI")
	TSubclassOf<UARWorldUserWidget> HealthBarWidgetClass;
	UPROPERTY(EditAnywhere, Category = "C++ | UI")
	TSubclassOf<UARDamagePopUp> DamagePopupWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Body")
	FName TeamColorParameterName = "PaintColor";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Body")
	FLinearColor TeamColor = FLinearColor(1.f, 0.f, 0.f, 1.f);

private:
	UPROPERTY()
	UARWorldUserWidget* HealthBarWidget;
};
