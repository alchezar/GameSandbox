// Copyright © 2024, Ivan Kinder

using UnrealBuildTool;

public class Project14 : ModuleRules
{
	public Project14(ReadOnlyTargetRules Target) : base(Target)
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
			"UMG",
			"Niagara",
			"EnhancedInput"
		});
	}
}