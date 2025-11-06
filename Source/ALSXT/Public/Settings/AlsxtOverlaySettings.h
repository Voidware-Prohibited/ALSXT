#pragma once

#include "GameplayTagContainer.h"
#include "Camera/CameraShakeBase.h"
#include "Animation/AnimInstance.h"
#include "Utility/AlsxtOverlayStructs.h"
#include "Settings/AlsxtCameraEffectsSettings.h"
#include "AlsxtOverlaySettings.generated.h"

struct FAlsxtMovementCameraShakeSettings;

UCLASS()
class ALSXT_API UAlsxtOverlaySettingsDataAsset: public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.OverlayMode"))
	TMap<FGameplayTag, FAlsxtOverlayAnimationInfo> Overlays;
};


USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtOverlaySettings
{
	GENERATED_BODY()

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


