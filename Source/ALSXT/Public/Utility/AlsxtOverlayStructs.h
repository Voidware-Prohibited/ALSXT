#pragma once

#include "NativeGameplayTags.h"
#include "Settings/AlsxtCameraEffectsSettings.h"
#include "Templates/SubclassOf.h"
#include "AlsxtOverlayStructs.generated.h"

class UAlsxtOverlaySettingsDataAsset;
class UAnimInstance;

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtOverlayAnimationStanceInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> RightHandAnimationInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool UseUniqueAnimInstanceForLeftHand {false};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta =(EditCondition="UseUniqueAnimInstanceForLeftHand", EditConditionHides=true))
	TSubclassOf<UAnimInstance> LeftHandAnimationInstance;

	bool operator==(const FAlsxtOverlayAnimationStanceInfo& other) const
	{
		return (other.RightHandAnimationInstance == RightHandAnimationInstance) && (other.UseUniqueAnimInstanceForLeftHand == UseUniqueAnimInstanceForLeftHand) && (other.LeftHandAnimationInstance == LeftHandAnimationInstance);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtOverlayAnimationInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAlsxtOverlayAnimationStanceInfo ActiveStance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FAlsxtOverlayAnimationStanceInfo NeutralStance;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FAlsxtOverlayAnimationStanceInfo PassiveStance;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<UAnimInstance> BothHandsAnimationInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.OverlaySlot", AllowPrivateAccess))
	FGameplayTagContainer AvailableSlots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Gait", AllowPrivateAccess))
	FGameplayTagContainer AvailableGaits;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Stance", AllowPrivateAccess))
	FGameplayTagContainer AvailableStances;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Ready Stance", AllowPrivateAccess))
	FGameplayTagContainer AvailableReadyStances;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.OverlaySettings", AllowPrivateAccess))
	FGameplayTagContainer Settings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UAlsxtCameraShakeSettings> OverlayCameraShakeSettings;

	bool operator==(const FAlsxtOverlayAnimationInfo& other) const
	{
		return (other.ActiveStance == ActiveStance) && (other.NeutralStance == NeutralStance) && (other.PassiveStance == PassiveStance) && (other.BothHandsAnimationInstance == BothHandsAnimationInstance) && (other.Settings == Settings);
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