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
			"AIModule",
			"EnhancedInput",
			"Niagara"
		});

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"NavigationSystem",
			"PhysicsCore",
			"UMG"
		});

		PublicIncludePaths.Add("Project10");
	}
}