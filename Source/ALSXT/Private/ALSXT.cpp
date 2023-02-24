// Copyright Epic Games, Inc. All Rights Reserved.

#include "ALSXT.h"
#include "Engine/CollisionProfile.h"

#define LOCTEXT_NAMESPACE "FALSXTModule"

void FALSXTModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	AddPhysAnimCollisionProfile();
}

void FALSXTModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
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