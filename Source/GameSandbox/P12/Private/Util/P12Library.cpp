// Copyright (C) 2023, IKinder

#include "P12/Public/Util/P12Library.h"

#include "P12/Public/Util/P12CoreTypes.h"
#include "HAL/IConsoleManager.h"

static TAutoConsoleVariable CVarP12DrawDebug(TEXT("P12.Debug"), 0, TEXT("Allow to draw debug helpers and console logs."), ECVF_Cheat);
static TAutoConsoleVariable CVarP12DrawDebugPrintScreen(TEXT("P12.Debug.PrintScreen"), 0, TEXT("Allow to print screen messages."), ECVF_Cheat);
static TAutoConsoleVariable CVarP12DrawDebugLegAlignment(TEXT("P12.Debug.LegAlignment"), 0, TEXT("Allow to draw leg alignment debug helpers."), ECVF_Cheat);
static TAutoConsoleVariable CVarP12DrawDebugLedgeDetection(TEXT("P12.Debug.LedgeDetection"), 0, TEXT("Allow to draw ledge detection debug helpers."), ECVF_Cheat);
static TAutoConsoleVariable CVarP12DrawDebugText(TEXT("P12.Debug.Text"), 0, TEXT("Allow to draw debug text helpers."), ECVF_Cheat);
static TAutoConsoleVariable CVarP12DrawDebugFire(TEXT("P12.Debug.Fire"), 0, TEXT("Allow to draw fire debug helpers."), ECVF_Cheat);

TArray<FString> UP12Library::GetAllConsoleVariables()
{
	TArray<FString> ConsoleVariables;
	ConsoleVariables.Add("P12.Debug");
	ConsoleVariables.Add("P12.Debug.PrintScreen");
	ConsoleVariables.Add("P12.Debug.LegAlignment");
	ConsoleVariables.Add("P12.Debug.LedgeDetection");
	ConsoleVariables.Add("P12.Debug.Text");
	ConsoleVariables.Add("P12.Debug.Fire");
	return ConsoleVariables;
}

bool UP12Library::GetCanDrawDebug()
{
	return IConsoleManager::Get().FindConsoleVariable(TEXT("P12.Debug"))->GetBool();
}

bool UP12Library::GetCanDrawDebugPrintScreen()
{
	return GetCanDrawDebug() && IConsoleManager::Get().FindConsoleVariable(TEXT("P12.Debug.PrintScreen"))->GetBool();
}

bool UP12Library::GetCanDrawDebugLegAlignment()
{
	return GetCanDrawDebug() && IConsoleManager::Get().FindConsoleVariable(TEXT("P12.Debug.LegAlignment"))->GetBool();
}

bool UP12Library::GetCanDrawDebugLedgeDetection()
{
	return GetCanDrawDebug() && IConsoleManager::Get().FindConsoleVariable(TEXT("P12.Debug.LedgeDetection"))->GetBool();
}

bool UP12Library::GetCanDrawDebugText()
{
	return GetCanDrawDebug() && IConsoleManager::Get().FindConsoleVariable(TEXT("P12.Debug.Text"))->GetBool();
}

bool UP12Library::GetCanDrawDebugFire()
{
	return GetCanDrawDebug() && IConsoleManager::Get().FindConsoleVariable(TEXT("P12.Debug.Fire"))->GetBool();
}

void UP12Library::DrawDebugLineTrace(const UWorld* World, const FHitResult& HitResult, const bool bDraw, const bool bOnTick)
{
	if (!bDraw || !World)
	{
		return;
	}
	if (!HitResult.bBlockingHit)
	{
		DrawDebugLine(World, HitResult.TraceStart, HitResult.TraceEnd, FColor::Green, false, bOnTick ? 0.f : 5.f);
		return;
	}
	DrawDebugLine(World, HitResult.TraceStart, HitResult.ImpactPoint, FColor::Red, false, bOnTick ? 0.f : 5.f);
	DrawDebugLine(World, HitResult.ImpactPoint, HitResult.TraceEnd, FColor::Green, false, bOnTick ? 0.f : 5.f);
	DrawDebugPoint(World, HitResult.ImpactPoint, 10.f, FColor::Red, false, bOnTick ? 0.f : 5.f);
}

void UP12Library::DrawDebugCapsuleTrace(const UWorld* World, const FHitResult& HitResult, const float Radius, const float HalfHeight, const FColor Color, const bool bDraw)
{
	if (!bDraw || !World)
	{
		return;
	}

	constexpr float DrawTime = 5.f;
	constexpr float SuccessThickness = 1.f;

	if (HitResult.bBlockingHit)
	{
		DrawDebugCapsule(World, HitResult.Location, HalfHeight, Radius, FQuat::Identity, Color, false, DrawTime, 0, SuccessThickness);
		DrawDebugDirectionalArrow(World, HitResult.TraceStart, HitResult.Location, Radius, Color, false, DrawTime, 0, SuccessThickness);
	}
	else
	{
		DrawDebugCapsule(World, HitResult.TraceEnd, HalfHeight, Radius, FQuat::Identity, Color, false, DrawTime);
		DrawDebugDirectionalArrow(World, HitResult.TraceStart, HitResult.TraceEnd, Radius, Color, false, DrawTime, 0, SuccessThickness);
	}
	DrawDebugCapsule(World, HitResult.TraceStart, HalfHeight, Radius, FQuat::Identity, Color, false, DrawTime);
}

void UP12Library::DrawDebugDirectionalCapsule(const UWorld* World, FP12LedgeDescription& LedgeDescription, const float Radius, const float HalfHeight, const FVector& Location, const bool bDraw)
{
	if (!bDraw || !World)
	{
		return;
	}
	DrawDebugCapsule(World, Location, HalfHeight, Radius, FQuat::Identity, FColor::Red, false, 5.f, 0, 1.f);
	DrawDebugDirectionalArrow(World, Location, Location + LedgeDescription.Rotation.Vector() * 50.f, 50.f, FColor::Red, false, 5.f, 0, 1.f);
}

void UP12Library::DrawDebugText(const UWorld* World, const FVector& Location, const FString& Text, const bool bDraw, const bool bOnTick)
{
	if (!bDraw || !World)
	{
		return;
	}
	DrawDebugString(World, Location, Text, nullptr, FColor::Green, bOnTick ? 0.f : 5.f, true);
}

void UP12Library::DrawPrintString(const UWorld* World, const FString& Text, const bool bDraw, const bool bOnTick)
{
	if (!bDraw || !World)
	{
		return;
	}
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, bOnTick ? 0.f : 5.f, FColor::Cyan, Text);

}

void UP12Library::DrawDebugSphereTrace(const UWorld* World, const FHitResult& HitResult, const float Radius, const FVector& Start, const FVector& End, const bool bDraw, const bool bOnTick)
{
	if (!bDraw || !World)
	{
		return;
	}
	const FVector Middle = Start + (End - Start) / 2.f;
	const float HalfHeight = FVector::Dist(Start, Middle) + Radius;
	const FRotator Rotation = (Start - End).ToOrientationRotator() + FRotator(90.f, 0.f, 0.f);
	DrawDebugCapsule(World, Middle, HalfHeight, Radius, Rotation.Quaternion(), FColor::Orange, false, bOnTick ? 0.f : 5.f);

	if (HitResult.bBlockingHit)
	{
		DrawDebugCapsule(World, HitResult.Location, Radius, Radius, FQuat::Identity, FColor::Orange, false, bOnTick ? 0.f : 5.f, 0, 1.f);
	}
}

void UP12Library::FixedTurn(FRotator& DesiredRotation, const FRotator& CurrentRotation, const FRotator& DeltaRot)
{
	/* PITCH */
	if (!FMath::IsNearlyEqual(CurrentRotation.Pitch, DesiredRotation.Pitch))
	{
		DesiredRotation.Pitch = FMath::FixedTurn(CurrentRotation.Pitch, DesiredRotation.Pitch, DeltaRot.Pitch);
	}
	/* YAW */
	if (!FMath::IsNearlyEqual(CurrentRotation.Yaw, DesiredRotation.Yaw))
	{
		DesiredRotation.Yaw = FMath::FixedTurn(CurrentRotation.Yaw, DesiredRotation.Yaw, DeltaRot.Yaw);
	}
	/* ROLL */
	if (!FMath::IsNearlyEqual(CurrentRotation.Roll, DesiredRotation.Roll))
	{
		DesiredRotation.Roll = FMath::FixedTurn(CurrentRotation.Roll, DesiredRotation.Roll, DeltaRot.Roll);
	}
}
