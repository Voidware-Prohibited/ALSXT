// Designed by Hitman's Store, 2023

#include "Settings/AlsxtProceduralRecoilSettingsDataAsset.h"

#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Settings/AlsxtAnimationModifierExtractCurvesSettings.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Components/Character/AlsxtProceduralRecoilAnimComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Misc/MessageDialog.h"
#include "UObject/SavePackage.h"

void UAlsxtProceduralRecoilSettingsDataAsset::SaveData()
{
	/** Getting the full path to the file */
	const FString FilePath = FPackageName::LongPackageNameToFilename(GetPackage()->GetPathName(),
																	 FPackageName::GetAssetPackageExtension());

	/** Finally saving just created package */
	FSavePackageArgs SavePackageArgs;

	SavePackageArgs.TopLevelFlags = EObjectFlags::RF_Public | EObjectFlags::RF_Standalone;
	SavePackageArgs.Error = GError;
	SavePackageArgs.bForceByteSwapping = true;
	SavePackageArgs.bWarnOfLongFilename = true;
	SavePackageArgs.SaveFlags = SAVE_NoError;

	GetPackage()->SetDirtyFlag(true);
	const bool bSuccess = UPackage::SavePackage(GetPackage(), this, *FilePath, SavePackageArgs);
	
	UE_LOG(LogAlsxtProceduralRecoil, Warning, TEXT("Saved Package: %s, location: %s"),
		   bSuccess ? TEXT("SUCCESS") : TEXT("ERROR"), *FilePath)
}
