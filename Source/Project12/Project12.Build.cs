// Copyright © 2024, Ivan Kinder

using UnrealBuildTool;

public class Project12 : ModuleRules
{
	public Project12(ReadOnlyTargetRules Target) : base(Target)
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