// Copyright © 2025, Ivan Kinder

#pragma once

class UP15BaseAbility;

DECLARE_MULTICAST_DELEGATE_OneParam(FP15OnAnimNotifySignature, USkeletalMeshComponent* /*SkeletalMesh*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FP15OnHealthChangedSignature, float /*Percentage*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FP15OnAbilityStartedSignature, TSubclassOf<UP15BaseAbility> /*AbilityClass*/);
