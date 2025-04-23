// Copyright © 2025, Ivan Kinder

#include "Actor/P16PointCollection.h"

#include "Project16.h"
#include "AbilitySystem/P16AbilitySystemLibrary.h"

AP16PointCollection::AP16PointCollection()
{
	Points.Reserve(P16::PointsNum);
	for (int32 Index = 0; Index < P16::PointsNum; ++Index)
	{
		FString          PointName = FString::Printf(L"Point%d", Index);
		USceneComponent* Point     = CreateDefaultSubobject<USceneComponent>(*PointName);

		if (Index == 0)
		{
			SetRootComponent(Point);
		}
		else
		{
			Point->SetupAttachment(GetRootComponent());
		}

		Points.Add(Point);
	}
}

void AP16PointCollection::BeginPlay()
{
	Super::BeginPlay();
}

TArray<USceneComponent*> AP16PointCollection::GetGroundPoints(const FVector& GroundLocation, int32 NumPoints, const float YawOverride)
{
	NumPoints                    = FMath::Min(NumPoints, Points.Num());
	constexpr int32 HeightOffset = 500.f;
	const FVector   RootLocation = Points[0]->GetComponentLocation();

	TArray<USceneComponent*> Result = {};
	for (int32 Index = 0; Index < NumPoints; ++Index)
	{
		USceneComponent* Point       = Points[Index];
		FVector          NewLocation = Point->GetComponentLocation();
		FRotator         NewRotation = Point->GetComponentRotation();

		// Rotate each point around the first one.
		if (Index > 0)
		{
			const FVector ToPoint = (NewLocation - RootLocation).RotateAngleAxis(YawOverride, FVector::UpVector);
			NewLocation           = RootLocation + ToPoint;
		}

		// Get point's location and rotation on the ground beneath.
		FHitResult            TraceResult;
		const FVector         Start = NewLocation + FVector::UpVector * HeightOffset;
		const FVector         End   = NewLocation - FVector::UpVector * HeightOffset;
		FCollisionQueryParams Params;
		Params.AddIgnoredActors(UP16AbilitySystemLibrary::GetLivePlayersWithinRadius(GetWorld(), {}, HeightOffset, RootLocation));
		if (GetWorld()->LineTraceSingleByProfile(TraceResult, Start, End, "BlockAll", Params))
		{
			NewLocation.Z = TraceResult.ImpactPoint.Z;
			NewRotation   = FRotationMatrix::MakeFromZ(TraceResult.ImpactNormal).Rotator();
		}

		// Collect points with updated location and rotation.
		Point->SetWorldLocation(NewLocation);
		Point->SetWorldRotation(NewRotation);;
		Result.Add(Point);
	}

	return Result;
}
