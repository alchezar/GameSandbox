// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Utils/P15Utils.h"
#include "P15HUD.generated.h"

struct FP15AbilityInfo;
class UP15PrimaryWidget;

UCLASS()
class PROJECT15_API AP15HUD : public AHUD
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
protected:
	virtual void BeginPlay() override;

	/* ------------------------------- This -------------------------------- */
public:
	void AddAbilityToUI(const int32 Index, FP15AbilityInfo&& AbilityInfo, FP15OnAbilityStartedSignature* OnAbilityStartedDelegate) const;

	/* ------------------------------ Fields ------------------------------- */
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TSubclassOf<UP15PrimaryWidget> PrimaryWidgetClass = nullptr;

private:
	TObjectPtr<UP15PrimaryWidget> PrimaryWidget = nullptr;
};
