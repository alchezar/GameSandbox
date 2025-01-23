// Copyright © 2024, Ivan Kinder

#include "Items/P14Battery.h"

P14::FBattery::FBattery(const float InPercent)
{
	SetPercent(InPercent);
}

std::partial_ordering P14::FBattery::operator<=>(const FBattery& RightBattery) const
{
	return std::partial_ordering{GetPercent() <=> RightBattery.GetPercent()};
}

bool P14::FBattery::operator==(const FBattery& RightBattery) const
{
	return FMath::IsNearlyEqual(GetPercent(), RightBattery.GetPercent());
}

void P14::FBattery::Charge()
{
	SetPercent(Percent + ChargeAmount);
}

void P14::FBattery::Discharge()
{
	SetPercent(Percent - ChargeAmount);
}

float P14::FBattery::GetPercent() const
{
	return Percent;
}

FColor P14::FBattery::GetColor() const
{
	if (Percent > 0.8f)
	{
		return FColor::Green;
	}
	if (Percent > 0.3f)
	{
		return FColor::Yellow;
	}
	return FColor::Red;
}

FString P14::FBattery::ToString() const
{
	return FString::Printf(TEXT("%i%%"), FMath::RoundToInt(Percent * 100));
}

void P14::FBattery::SetPercent(const float InPercent)
{
	Percent = FMath::Clamp(InPercent, 0.f, 1.f);
}
