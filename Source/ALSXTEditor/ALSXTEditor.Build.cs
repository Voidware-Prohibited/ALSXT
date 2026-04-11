// MIT

using System.Linq;
using UnrealBuildTool;

public class ALSXTEditor: ModuleRules
{
	public ALSXTEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		// Toggle optimization based on a custom flag
		// To Activate, run build with: -define:WITH_COVERAGE=1
		if (Target.GlobalDefinitions.Contains("WITH_COVERAGE=1"))
		{
			OptimizeCode = CodeOptimization.Never;
			bUseUnity = false;
			
			if (Target.Platform == UnrealTargetPlatform.Win64)
			{
				// /Ob0: Disables inline expansion
				// /Od: Disables optimizations (optional, but often needed with /Ob0)
				// PrivateCompilerFlags.Add("/Ob0"); 
			}
			
			if (Target.Platform == UnrealTargetPlatform.Mac || Target.Platform == UnrealTargetPlatform.Linux)
			{
				// PrivateCompilerFlags.Add("-fno-inline");
			}
		}
		
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