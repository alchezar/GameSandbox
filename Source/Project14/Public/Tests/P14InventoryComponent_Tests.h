// Copyright © 2024, Ivan Kinder

#pragma once
#include "Components/P14InventoryComponent.h"

#if (WITH_DEV_AUTOMATION_TESTS || WITH_PERF_AUTOMATION_TESTS)

namespace P14::Test
{
	class UP14InventoryComponent_Testable : public UP14InventoryComponent
	{
	public:
		void SetLimit(const EP14InventoryItemType InType, const int32 InLimit)
		{
			if (!InventoryLimits.Contains(InType))
			{
				InventoryLimits.Add(InType, 0);
			}

			InventoryLimits[InType] = InLimit;
		}
	};
}

#endif
