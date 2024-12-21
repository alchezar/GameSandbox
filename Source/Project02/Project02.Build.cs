// Copyright © 2024, Ivan Kinder

using UnrealBuildTool;

public class Project02 : ModuleRules
{
	public Project02(ReadOnlyTargetRules Target) : base(Target)
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
			"Niagara",
			"PhysicsCore",
			"UMG",
		});
	}
}