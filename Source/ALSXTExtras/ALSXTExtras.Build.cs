// MIT

using System.Linq;
using System.Reflection;
using UnrealBuildTool;

public class ALSXTExtras : ModuleRules
{
	public ALSXTExtras(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		
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

		CppCompileWarningSettings.NonInlinedGenCppWarningLevel = WarningLevel.Warning;

		PublicDependencyModuleNames.AddRange([
			"Core",
			"Landscape",
			"CoreUObject",
			"Engine",
			"UMG",
			"GameplayTags",
			"GameplayAbilities",
			"ModularGameplayActors"
		]);			
		
		PrivateDependencyModuleNames.AddRange([
			"PhysicsCore",
			"Slate",
			"SlateCore"
		]);

		if (Target.Type == TargetRules.TargetType.Editor)
		{
			PrivateDependencyModuleNames.AddRange([
				"MessageLog"
			]);
		}

		SetupIrisSupport(Target);
	}
}
