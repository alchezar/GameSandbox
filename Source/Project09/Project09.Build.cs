// Copyright © 2024, Ivan Kinder

using UnrealBuildTool;

public class Project09 : ModuleRules
{
	public Project09(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore"
		});

		PrivateDependencyModuleNames.AddRange(new string[] 
		{
		});
	}
}