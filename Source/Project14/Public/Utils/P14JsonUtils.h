// Copyright © 2025, Ivan Kinder

#pragma once
#include "P14InputRecordingTypes.h"

namespace P14::Test
{
	class FJsonUtils
	{
	public:
		static bool WriteInputData(const FString& FileName, const FP14InputData& InputData);
		static bool ReadInputData(const FString& FileName, FP14InputData& InputData);
	};
}
