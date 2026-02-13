#pragma once

#include "NativeGameplayTags.h"
#include "AlsxtChooserAnimationCriteria.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtChooserCriteriaLocomotion
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Criteria|Locomotion", Meta = (Categories = "Als.Locomotion Variant,Als.Status Locomotion Variant,Als.LocomotionModifier,Als.Overlay Mode,Als.Stance,Als.Combat Stance,Als.Injury", AllowPrivateAccess))
	FGameplayTagContainer Criteria;
	

	bool operator==(const FAlsxtChooserCriteriaLocomotion& other) const
	{
		return (other.Criteria == Criteria);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtChooserCriteriaOverlay
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Criteria|Locomotion", Meta = (Categories = "Als.OverlayMode,Als.OverlaySlot,Als.OverlaySlotStance,Als.OverlayLeftHandMirrorPolicy,Als.OverlaySettings", AllowPrivateAccess))
	FGameplayTagContainer Criteria;

	bool operator==(const FAlsxtChooserCriteriaOverlay& other) const
	{
		return (other.Criteria == Criteria);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtChooserCriteriaAdditiveIdle
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Criteria|Locomotion", Meta = (Categories = "Als.OverlayMode,Als.OverlaySlot,Als.OverlaySlotStance,Als.OverlayLeftHandMirrorPolicy,Als.OverlaySettings", AllowPrivateAccess))
	FGameplayTagContainer Criteria;

	bool operator==(const FAlsxtChooserCriteriaOverlay& other) const
	{
		return (other.Criteria == Criteria);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtChooserCriteriaLocomotionAction
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Criteria|Locomotion", Meta = (Categories = "Als.Locomotion Action,Als.Locomotion Variant,Als.OverlayMode,Als.Combat Stance,Als.Action Strength,Als.Mantle Type,Als.LocomotionActionSettings", AllowPrivateAccess))
	FGameplayTagContainer Criteria;

	bool operator==(const FAlsxtChooserCriteriaLocomotionAction& other) const
	{
		return (other.Criteria == Criteria);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtChooserCriteriaMantle
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Criteria|Locomotion", Meta = (Categories = "Als.Locomotion Variant,Als.OverlayMode,Als.Combat Stance,Als.Mantle Type", AllowPrivateAccess))
	FGameplayTagContainer Criteria;

	bool operator==(const FAlsxtChooserCriteriaMantle& other) const
	{
		return (other.Criteria == Criteria);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtChooserCriteriaVault
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Criteria|Locomotion", Meta = (Categories = "Als.Locomotion Variant,Als.OverlayMode,Als.Combat Stance,Als.Vault Type", AllowPrivateAccess))
	FGameplayTagContainer Criteria;

	bool operator==(const FAlsxtChooserCriteriaVault& other) const
	{
		return (other.Criteria == Criteria);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtChooserCriteriaGesture
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Criteria|Locomotion", Meta = (Categories = "Als.Gesture,Als.Hand", AllowPrivateAccess))
	FGameplayTagContainer Criteria;

	bool operator==(const FAlsxtChooserCriteriaGesture& other) const
	{
		return (other.Criteria == Criteria);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtChooserCriteriaFingerPose
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Criteria|Locomotion", Meta = (Categories = "Als.Firearm Finger Pose,Als.Hand", AllowPrivateAccess))
	FGameplayTagContainer Criteria;

	bool operator==(const FAlsxtChooserCriteriaFingerPose& other) const
	{
		return (other.Criteria == Criteria);
	}
};