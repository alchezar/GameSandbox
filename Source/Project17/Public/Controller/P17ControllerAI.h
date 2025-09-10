// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "P17ControllerAI.generated.h"

class UAISenseConfig_Sight;

UCLASS()
class PROJECT17_API AP17ControllerAI : public AAIController
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
public:
	explicit AP17ControllerAI(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

	/// ------------------------------------------------------------------------
	/// @name Interface
	/// ------------------------------------------------------------------------
public:
	/// @par IGenericTeamAgentInterface ----------------------------------------
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
protected:
	UFUNCTION()
	void OnTargetPerceptionUpdatedCallback(AActor* Actor, FAIStimulus Stimulus);

private:
	void SetTeamId(const uint8 TeamId);
	ECrowdAvoidanceQuality::Type SelectAvoidanceQuality() const;

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++|Component")
	TObjectPtr<UAISenseConfig_Sight> SenseSight = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++|Component")
	TObjectPtr<UAIPerceptionComponent> PerceptionComp = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++|Crowd")
	bool bEnableDetourCrowdAvoidance = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++|Crowd", meta = (EditCondition = "bEnableDetourCrowdAvoidance", UIMin = 1, UIMax = 4))
	int32 DetourCrowdAvoidanceQuality = 4;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++|Crowd", meta = (EditCondition = "bEnableDetourCrowdAvoidance"))
	float CollisionQueryRange = 500.f;
};
