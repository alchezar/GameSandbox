// Copyright © 2025, Ivan Kinder

#pragma once

DECLARE_MULTICAST_DELEGATE_OneParam(FP15OnAnimNotifySignature, USkeletalMeshComponent* /*SkeletalMesh*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FP15OnHealthChangedSignature, float /*Percentage*/);
