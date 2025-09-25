#pragma once

#include "NativeGameplayTags.h"
#include "Templates/SubclassOf.h"
#include "Settings/AlsxtOverlaySettings.h"
#include "AlsxtOverlayStructs.generated.h"

class UAnimInstance;

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtOverlayAnimationInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.OverlayMode,Als.OverlaySlot,Als.OverlaySlotStance,Als.OverlayLeftHandMirrorPolicy,Als.OverlaySettings", AllowPrivateAccess))
	FGameplayTagContainer OverlayTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> AnimationInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAlsxtOverlayCameraShakeSettings CameraShakeSettings;

	bool operator==(const FAlsxtOverlayAnimationInfo& other) const
	{
		return (other.OverlayTags == OverlayTags) && (other.AnimationInstance == AnimationInstance);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtOverlaySlots
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.OverlayMode,Als.OverlaySlotStance,Als.OverlayLeftHandMirrorPolicy,Als.OverlaySettings", AllowPrivateAccess))
	FGameplayTagContainer OverlaySlotRightHand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.OverlayMode,Als.OverlaySlotStance,Als.OverlayLeftHandMirrorPolicy,Als.OverlaySettings", AllowPrivateAccess))
	FGameplayTagContainer OverlaySlotLeftHand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.OverlayMode,Als.Als.OverlaySettings", AllowPrivateAccess))
	FGameplayTagContainer OverlaySlotBothHands;

	bool operator==(const FAlsxtOverlaySlots& other) const
	{
		return (other.OverlaySlotRightHand == OverlaySlotRightHand) && (other.OverlaySlotLeftHand == OverlaySlotLeftHand) && (other.OverlaySlotBothHands == OverlaySlotBothHands);
	}
};