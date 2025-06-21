using UnrealBuildTool;

public class Project17 : ModuleRules
{
	public Project17(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core",
			"GameplayTags",
			"EnhancedInput"
		});

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"CoreUObject",
			"Engine",
			"Slate",
			"SlateCore"
		});

		PublicIncludePaths.Add("Project17");
	}
}