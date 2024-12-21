// Copyright © 2024, Ivan Kinder

using UnrealBuildTool;

public class Project04 : ModuleRules
{
	public Project04(ReadOnlyTargetRules Target) : base(Target)
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
			"AIModule",
			"EnhancedInput",
			"NavigationSystem",
			"UMG",
		});
	}
}