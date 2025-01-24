// Copyright © 2024, Ivan Kinder

using UnrealBuildTool;

public class Project05 : ModuleRules
{
	public Project05(ReadOnlyTargetRules Target) : base(Target)
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
			"AnimGraphRuntime",
			"EnhancedInput",
			"Niagara",
			"ProceduralMeshComponent"
		});
	}
}