// Copyright © 2024, Ivan Kinder

using UnrealBuildTool;

public class Project13 : ModuleRules
{
	public Project13(ReadOnlyTargetRules Target) : base(Target)
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
			"Niagara",
			"OnlineSubsystem",
			"PhysicsCore",
			"Slate",
			"SlateCore",
			"UMG"
		});

		PublicIncludePaths.Add("Project13");
	}
}