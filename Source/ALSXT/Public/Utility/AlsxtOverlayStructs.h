#pragma once

#include "AlsxtLinkedAnimationInstance.h"
#include "NativeGameplayTags.h"
#include "Settings/AlsxtDefensiveModeSettings.h"
#include "Templates/SubclassOf.h"
#include "AlsxtOverlayStructs.generated.h"

class UAnimInstance;

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtOverlayInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.OverlayMode, Als.OverlaySlot, Als.OverlayLeftHandMirrorPolicy", AllowPrivateAccess))
	FGameplayTagContainer OverlayTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> AnimationInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.OverlaySettings", AllowPrivateAccess))
	FGameplayTagContainer OverlaySettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAlsxtOverlayCameraShakeSettings CameraShakeSettings;

	bool operator==(const FAlsxtOverlayInfo& other) const
	{
		return (other.OverlayTags == OverlayTags) && (other.AnimationInstance == AnimationInstance);
	}
};