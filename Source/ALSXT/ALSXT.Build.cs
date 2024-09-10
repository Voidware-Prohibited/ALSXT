// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ALSXT : ModuleRules
{
	public ALSXT(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
		bUseUnity = false;

		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core", "Landscape", "CoreUObject", "Engine", "AudioExtensions", "UMG", "GameplayTags", "GameplayCameras", "CinematicCamera", "AIModule", "AnimGraphRuntime", "RigVM", "ControlRig", "EnhancedInput", "ALS", "ALSCamera", "Niagara",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"EngineSettings",
				"NetCore",
				"PhysicsCore",
				"Slate",
				"SlateCore",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);

		if (Target.Type == TargetRules.TargetType.Editor)
		{
			PrivateDependencyModuleNames.AddRange(new[]
			{
				"MessageLog"
			});
		}

		SetupIrisSupport(Target);
	}
}
