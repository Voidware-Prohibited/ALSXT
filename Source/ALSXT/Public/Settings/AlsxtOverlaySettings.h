#pragma once

#include "AlsxtMovementSettings.h"
#include "GameplayTagContainer.h"
#include "Camera/CameraShakeBase.h"
#include "Animation/AnimInstance.h"
#include "Utility/AlsxtOverlayStructs.h"
#include "Settings/AlsxtCameraEffectsSettings.h"
#include "Settings/AlsxtViewSettings.h"
#include "Chooser.h"
#include "Utility/AlsGameplayTags.h"
#include "Settings/AlsxtAnimationSequenceSettings.h"
#include "AlsxtOverlaySettings.generated.h"

class UAlsxtAimableOverlayObjectLookupTableDataAsset;
class UAlsxtOverlayObjectLookupTableDataAsset;
class UAlsxtOverlayLookupTableDataAsset;
struct FAlsxtMovementCameraShakeSettings;

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtOverlaySettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Overlay Modes")
	TSoftObjectPtr<UChooserTable> OverlayChooserTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TSoftObjectPtr<UAlsxtOverlayLookupTableDataAsset> OverlayLookupTable {nullptr};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TSoftObjectPtr<UAlsxtOverlayObjectLookupTableDataAsset> OverlayObjectLookupTable {nullptr};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TSoftObjectPtr<UAlsxtAimableOverlayObjectLookupTableDataAsset> AimableOverlayObjectLookupTable {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	TSoftObjectPtr<UAlsxtPoseMapSettingsAsset> OverlayPoseMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Overlay Modes")
	TSoftObjectPtr<UChooserTable> OverlayActionChooserTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Overlay Modes")
	TSoftObjectPtr<UChooserTable> LocomotionActionChooserTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Stance")
	bool bEnableOverlaySwitching {true};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Stance", Meta = (Categories = "Als.Locmotion Variant"))
	FGameplayTag DefaultOverlayMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bOverrideLocomotionVariant {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Locmotion Variant", EditCondition = "bOverrideLocomotionVariant"))
	FGameplayTag LocomotionVariant;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ForceInlineRow))
	FAlsxtMovementRotationModeSettings MovementSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.OverlayMode"))
	TMap<FGameplayTag, TSubclassOf<UAnimInstance>> OverlayAnimationInstanceClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.OverlayMode"))
	TSoftObjectPtr<UAlsxtOverlaySettingsDataAsset> Overlays;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer AimableOverlayModes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseGunBoneForOverlayObjects {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer LeftIKOverlayModes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGameplayTag, TObjectPtr<UAlsxtCameraShakeSettings>> CameraShake;
};

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UAlsxtOverlaySettingsDataAsset: public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.OverlayMode"))
	TMap<FGameplayTag, FAlsxtOverlayAnimationInfo> Overlays;

	// General Settings for Gameplay Camera System
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ForceInlineRow))
	TSoftObjectPtr<UAlsxtGeneralViewSettingsDataAsset> ViewCameraSettings;
};

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UAlsxtOverlayDataAsset: public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ForceInlineRow))
	FAlsxtOverlayInfo OverlaySettings;
};
