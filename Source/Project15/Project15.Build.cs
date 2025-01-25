// Copyright © 2025, Ivan Kinder

using UnrealBuildTool;

public class Project15 : ModuleRules
{
	public Project15(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput"
		});

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"Niagara",
			"UMG"
		});

		PublicIncludePaths.Add("Project15");
	}
}