// Copyright © 2024, Ivan Kinder

using UnrealBuildTool;

public class Project01 : ModuleRules
{
	public Project01(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore"
		});

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"EnhancedInput",
			"UMG"
		});
	}
}