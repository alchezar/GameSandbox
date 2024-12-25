// Copyright © 2024, Ivan Kinder

#pragma once

namespace P14
{
	class PROJECT14_API FBattery
	{
	public:
		FBattery() = default;
		explicit FBattery(const float InPercent);

		std::partial_ordering operator<=>(const FBattery& RightBattery) const;
		bool                  operator==(const FBattery& RightBattery) const;

		void Charge();
		void Discharge();

		_NODISCARD float   GetPercent() const;
		_NODISCARD FColor  GetColor() const;
		_NODISCARD FString ToString() const;

	private:
		void SetPercent(const float InPercent);

	private:
		float Percent      = 1.f;
		float ChargeAmount = 0.1f;
	};
}
