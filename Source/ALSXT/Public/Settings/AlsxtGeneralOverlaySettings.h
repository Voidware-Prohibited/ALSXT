#pragma once

#include "AlsxtMovementSettings.h"
#include "AlsxtOverlaySettings.h"
#include "GameplayTagContainer.h"
#include "Camera/CameraShakeBase.h"
#include "Animation/AnimInstance.h"
#include "Utility/AlsxtOverlayStructs.h"
#include "Settings/AlsxtCameraEffectsSettings.h"
#include "Chooser.h"
#include "Utility/AlsGameplayTags.h"
#include "AlsxtGeneralOverlaySettings.generated.h"

struct FAlsxtOverlayAnimationInfo;
struct FAlsxtMovementCameraShakeSettings;

UCLASS()
class ALSXT_API UAlsxtGeneralOverlaySettingsDataAsset: public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.OverlayMode"))
	TMap<FGameplayTag, FAlsxtOverlayAnimationInfo> Overlays;
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtGeneralOverlaySettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Overlay Modes")
	TSoftObjectPtr<UChooserTable> OverlayChooserTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Overlay Modes")
	TSoftObjectPtr<UChooserTable> LocomotionActionChooserTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bOverrideLocomotionVariant {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Locmotion Variant", EditCondition = "bOverrideLocomotionVariant"))
	FGameplayTag LocomotionVariant;

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


