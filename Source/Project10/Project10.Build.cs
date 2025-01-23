// Copyright © 2024, Ivan Kinder

using UnrealBuildTool;

public class Project10 : ModuleRules
{
	public Project10(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"Niagara",
			"EnhancedInput",
			"AIModule"
		});

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"UMG",
			"NavigationSystem",
			"PhysicsCore"
		});
	}
}