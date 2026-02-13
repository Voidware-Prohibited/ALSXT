// MIT

using UnrealBuildTool;

public class ALSXTEditor: ModuleRules
{
	public ALSXTEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
			}
			);
		
		PrivateIncludePaths.AddRange(
			new string[] {
			}
		);
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core", "ALSXT"
			}
			);
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"AnimationModifiers", 
				"AnimationBlueprintLibrary",
				"BlueprintGraph",
				"AnimGraphRuntime",
				"AnimGraph",
				"GraphEditor"
			}
			);
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
			}
			);
	}
}