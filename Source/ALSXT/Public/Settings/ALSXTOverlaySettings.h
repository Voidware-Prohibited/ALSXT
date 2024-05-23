#pragma once

#include "ALSXTOverlaySettings.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTOverlaySettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGameplayTag, TSubclassOf<UAnimInstance>> OverlayAnimationInstanceClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer AimableOverlayModes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseGunBoneForOverlayObjects {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer LeftIKOverlayModes;
};
