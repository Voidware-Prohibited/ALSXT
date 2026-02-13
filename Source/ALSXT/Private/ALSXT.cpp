// Copyright Epic Games, Inc. All Rights Reserved.

#include "ALSXT.h"
#include "Engine/CollisionProfile.h"
#include "Developer/Settings/Public/ISettingsModule.h"
#include "Settings/AlsxtAnimationModifierExtractCurvesSettings.h"

#define LOCTEXT_NAMESPACE "FALSXTModule"

void FALSXTModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	AddPhysAnimCollisionProfile();
	if(ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings("Project", "Plugins", "ALSXT",
										 LOCTEXT("RuntimeSettingsName", "ALSXT Procedural Recoil Animation"),
										 LOCTEXT("RuntimeSettingsDescription", "ALSXT Procedural Recoil Animation Settings"),
										 GetMutableDefault<UAlsxtAnimationModifierExtractCurvesSettings>());
	}
}

void FALSXTModule::ShutdownModule()
{
	if(ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "ALSXT");
	}
}

void FALSXTModule::AddPhysAnimCollisionProfile()
{
	// Make sure PhysAnimCollisionProfileName is added to Engine's collision profiles
	const FName PhysAnimCollisionProfileName = "PhysicalAnimation";
	FCollisionResponseTemplate PhysAnimCollisionProfile;
	if (!UCollisionProfile::Get()->GetProfileTemplate(PhysAnimCollisionProfileName, PhysAnimCollisionProfile))
	{
		PhysAnimCollisionProfile.Name = PhysAnimCollisionProfileName;
		PhysAnimCollisionProfile.CollisionEnabled = ECollisionEnabled::PhysicsOnly;
		PhysAnimCollisionProfile.ObjectType = ECollisionChannel::ECC_WorldStatic;
		PhysAnimCollisionProfile.bCanModify = false;
		PhysAnimCollisionProfile.ResponseToChannels = ECR_Ignore;
		PhysAnimCollisionProfile.ResponseToChannels.Camera = ECR_Block;
		PhysAnimCollisionProfile.ResponseToChannels.Visibility = ECR_Ignore;
		PhysAnimCollisionProfile.ResponseToChannels.WorldStatic = ECR_Block;
		PhysAnimCollisionProfile.ResponseToChannels.WorldDynamic = ECR_Block;
		PhysAnimCollisionProfile.ResponseToChannels.Pawn = ECR_Ignore;
		PhysAnimCollisionProfile.ResponseToChannels.PhysicsBody = ECR_Ignore;
		PhysAnimCollisionProfile.ResponseToChannels.Destructible = ECR_Ignore;
		PhysAnimCollisionProfile.ResponseToChannels.Vehicle = ECR_Ignore;
#if WITH_EDITORONLY_DATA
		PhysAnimCollisionProfile.HelpMessage = TEXT("Default Physical Animation Collision Profile (Created by ALSXT)");
#endif

		FCollisionProfilePrivateAccessor::AddProfileTemplate(PhysAnimCollisionProfile);
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FALSXTModule, ALSXT)