// Copyright (C) 2023, IKinder

#include "P11/Public/UI/P11CharUI.h"

// #include "Animation/AnimSingleNodeInstance.h"
#include "Animation/AnimationAsset.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/SpotLightComponent.h"
#include "P11/Public/Game/P11SavePlayerInfo.h"

AP11CharUI::AP11CharUI()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);

	Light = CreateDefaultSubobject<USpotLightComponent>("SpotLight");
	Light->SetupAttachment(RootComponent);
	
	Capture = CreateDefaultSubobject<USceneCaptureComponent2D>("Capture2D");
	Capture->SetupAttachment(RootComponent);
	FMinimalViewInfo ViewInfo;
	ViewInfo.FOV = 45.f;
	ViewInfo.DesiredFOV = 45.f;
	Capture->SetCameraView(ViewInfo);

	FindDefaultReferences();
}

void AP11CharUI::BeginPlay()
{
	Super::BeginPlay();
	Capture->ShowOnlyActorComponents(this);
}

void AP11CharUI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP11CharUI::FindDefaultReferences() const
{
	if (!Mesh->GetSkeletalMeshAsset())
	{
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshFinder(TEXT("/Game/Characters/Jedihunter/Mesh/Jedihunter"));
		if (SkeletalMeshFinder.Succeeded())
		{
			Mesh->SetSkeletalMesh(SkeletalMeshFinder.Object);
		}
	}
	// if (!Mesh->GetAnimInstance())
	// {
	// 	static ConstructorHelpers::FObjectFinder<UAnimSequence> AnimationFinder(TEXT("/Game/Characters/Jedihunter/Animation/EndlessRunner/Clip/ER_Idle"));
	// 	if (AnimationFinder.Succeeded())
	// 	{
	// 		Mesh->SetAnimation(AnimationFinder.Object);
	// 	}
	// }
}

void AP11CharUI::ChoseSide(const EP11PlayerSide NewSide) const
{
	const FLinearColor NewColor = NewSide == EP11PlayerSide::Jedi ? FLinearColor::White : FLinearColor::Red;
	
	TArray<UMaterialInstanceDynamic*> DynamicMaterials;
	DynamicMaterials.Add(Mesh->CreateAndSetMaterialInstanceDynamic(1));
	DynamicMaterials.Add(Mesh->CreateAndSetMaterialInstanceDynamic(2));
	DynamicMaterials.Add(Mesh->CreateAndSetMaterialInstanceDynamic(3));
	
	for (const auto& DynamicMaterial : DynamicMaterials)
	{
		DynamicMaterial->SetVectorParameterValue("MainColor", NewColor);
		DynamicMaterial->SetVectorParameterValue("PaintColor", NewColor);
	}
}
