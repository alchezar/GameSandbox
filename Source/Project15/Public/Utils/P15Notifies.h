// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P15Utils.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "P15Notifies.generated.h"

///
/// @class UP15GroundBlastAnimNotify
/// @brief Custom anim notify class.
/// @details Extends the base class with the ability to broadcast a delegate when the notifier is fired.
///
UCLASS()
class PROJECT15_API UP15GroundBlastAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	FP15OnAnimNotifySignature Delegate;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override
	{
		Super::Notify(MeshComp, Animation, EventReference);
		Delegate.Broadcast(MeshComp);
	}
};

///
/// @class UP15GroundBlastAnimNotifyWindow
/// @brief Custom anim notify window class.
/// @details Extends the base class with the ability to broadcast a delegate when the notifiers are fired.
///
UCLASS()
class PROJECT15_API UP15GroundBlastAnimNotifyWindow : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	FP15OnAnimNotifySignature BeginDelegate;
	FP15OnAnimNotifySignature EndDelegate;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override
	{
		Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
		BeginDelegate.Broadcast(MeshComp);
	}

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override
	{
		Super::NotifyEnd(MeshComp, Animation, EventReference);
		EndDelegate.Broadcast(MeshComp);
	}
};

///
/// @class UP15FireBlastAnimNotifyWindow
/// @brief Custom anim notify window class.
/// @details Extends the base class with the ability to broadcast a delegate when the notifiers are fired.
///
UCLASS()
class PROJECT15_API UP15FireBlastAnimNotifyWindow : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	FP15OnAnimNotifySignature BeginDelegate;
	FP15OnAnimNotifySignature EndDelegate;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override
	{
		Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
		BeginDelegate.Broadcast(MeshComp);
	}

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override
	{
		Super::NotifyEnd(MeshComp, Animation, EventReference);
		EndDelegate.Broadcast(MeshComp);
	}
};
