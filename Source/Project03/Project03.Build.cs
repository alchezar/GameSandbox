// Copyright © 2024, Ivan Kinder

using UnrealBuildTool;

public class Project03 : ModuleRules
{
	public Project03(ReadOnlyTargetRules Target) : base(Target)
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