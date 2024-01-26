// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "P13LegAlignmentComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMESANDBOX_API UP13LegAlignmentComponent : public UActorComponent
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
public:
	UP13LegAlignmentComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* ------------------------------- This -------------------------------- */
public:
	FORCEINLINE float GetIKLeftLegOffset()  const { return IKLeftLegOffset; }
	FORCEINLINE float GetIKRightLegOffset() const { return IKRightLegOffset; }
	FORCEINLINE float GetIKHipOffset()      const { return IKHitOffset; }
	void InitLegAlignment(const FName& InLeftVirtualBoneName, const FName& InRightVirtualBoneName, const bool bStart = true);
	void LegAlignment(const bool bEnable);

private:
	float GetIKSocketOffset(const FName& VirtualBoneName, const float TraceHalfDistance = 50.f, const float FromBoneToBottom = 10.f) const;
	void LegsIKFloorAlignment();

	/* ----------------------------- Variables ----------------------------- */
private:
	TSoftObjectPtr<ACharacter> CachedCharacter;

	FName LeftVirtualBoneName = "VB VB SK_Jedihunter_root_l_ankle";
	FName RightVirtualBoneName = "VB VB SK_Jedihunter_root_r_ankle";

	float IKLeftLegOffset = 0.f;
	float IKRightLegOffset = 0.f;
	float IKHitOffset = 0.f;
};
