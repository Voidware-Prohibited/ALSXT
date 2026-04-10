#pragma once

#include "NativeGameplayTags.h"
#include "Settings/AlsxtCameraEffectsSettings.h"
#include "Settings/AlsxtGameplayCameraComponentSettings.h"
#include "Settings/AlsxtMovementSettings.h"
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
	FGameplayTagContainer AvailableGaits {AlsGaitTags::Walking};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Stance", AllowPrivateAccess))
	FGameplayTagContainer AvailableStances {AlsStanceTags::Standing};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Alsxt.CombatStance", AllowPrivateAccess))
	FGameplayTagContainer AvailableCombatStances {ALSXTCombatStanceTags::Orthodox};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Ready Stance", AllowPrivateAccess))
	FGameplayTagContainer AvailableReadyStances {AlsxtReadyStanceTags::Relaxed};

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
struct ALSXT_API FAlsxtRotationLerpGaitSettings
{
	GENERATED_BODY()

public:
	// Gait amount to acceleration, deceleration, and ground friction curve.
	// Gait amount ranges from 0 to 3, where 0 is stopped, 1 is walking, 2 is running, and 3 is sprinting.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	TObjectPtr<UCurveVector> AccelerationAndDecelerationAndGroundFrictionCurve;

	// Gait amount to rotation interpolation speed curve.
	// Gait amount ranges from 0 to 3, where 0 is stopped, 1 is walking, 2 is running, and 3 is sprinting.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	TObjectPtr<UCurveFloat> RotationInterpolationSpeedCurve;
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtRotationLerpStanceSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ForceInlineRow))
	TMap<FGameplayTag, FAlsxtRotationLerpGaitSettings> Stances
	{
			{AlsStanceTags::Standing, {}},
			{AlsStanceTags::Crouching, {}}
	};
};

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UAlsxtRotationLerpSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ForceInlineRow))
	TMap<FGameplayTag, FAlsxtRotationLerpStanceSettings> RotationModes
	{
				{AlsRotationModeTags::VelocityDirection, {}},
				{AlsRotationModeTags::ViewDirection, {}},
				{AlsRotationModeTags::Aiming, {}}
	};
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtOverlayInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
    int Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	FAlsxtOverlayAnimationInfo AnimationInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera")
	TSoftObjectPtr<UAlsxtCameraShakeSettings> CameraShakeSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera")
	TSoftObjectPtr<UAlsxtOverlayCameraOffsetSettings> CameraOffsetSettings;

	// Rotation Lag
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	TSoftObjectPtr<UAlsxtRotationLerpSettings> RotationLerpSettings;

	// Movement Speeds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	TSoftObjectPtr<UAlsxtMovementSettings> MovementSettings;

	bool operator==(const FAlsxtOverlayInfo& other) const
	{
		return (other.AnimationInfo == AnimationInfo) && (other.CameraShakeSettings == CameraShakeSettings) && (other.CameraOffsetSettings == CameraOffsetSettings);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtOverlayObjectInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	int Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	FAlsxtOverlayAnimationInfo AnimationInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera")
	TSoftObjectPtr<UAlsxtCameraShakeSettings> CameraShakeSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera")
	TSoftObjectPtr<UAlsxtOverlayCameraOffsetSettings> CameraOffsetSettings;

	// Rotation Lag
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	TSoftObjectPtr<UAlsxtRotationLerpSettings> RotationLerpSettings;

	// Movement Speeds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	TSoftObjectPtr<UAlsxtMovementSettings> MovementSettings;

	bool operator==(const FAlsxtOverlayInfo& other) const
	{
		return (other.AnimationInfo == AnimationInfo) && (other.CameraShakeSettings == CameraShakeSettings) && (other.CameraOffsetSettings == CameraOffsetSettings);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtAimableOverlayObjectInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	int Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	FAlsxtOverlayAnimationInfo AnimationInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera")
	TSoftObjectPtr<UAlsxtCameraShakeSettings> CameraShakeSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera")
	TSoftObjectPtr<UAlsxtOverlayCameraOffsetSettings> CameraOffsetSettings;

	// Rotation Lag
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	TSoftObjectPtr<UAlsxtRotationLerpSettings> RotationLerpSettings;

	// Movement Speeds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	TSoftObjectPtr<UAlsxtMovementSettings> MovementSettings;

	bool operator==(const FAlsxtOverlayInfo& other) const
	{
		return (other.AnimationInfo == AnimationInfo) && (other.CameraShakeSettings == CameraShakeSettings) && (other.CameraOffsetSettings == CameraOffsetSettings);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtOverlayLayers
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.OverlayMode,Als.OverlaySlotStance,Als.OverlayLeftHandMirrorPolicy,Als.OverlaySettings", AllowPrivateAccess))
	FGameplayTagContainer ActiveLayers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.OverlayMode,Als.OverlaySlotStance,Als.OverlayLeftHandMirrorPolicy,Als.OverlaySettings", AllowPrivateAccess))
	FGameplayTag OverlayLayerRightHand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.OverlayMode,Als.OverlaySlotStance,Als.OverlayLeftHandMirrorPolicy,Als.OverlaySettings", AllowPrivateAccess))
	FGameplayTag OverlayLayerLeftHand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.OverlayMode,Als.Als.OverlaySettings", AllowPrivateAccess))
	FGameplayTag OverlayLayerBothHands;

	bool operator==(const FAlsxtOverlayLayers& other) const
	{
		return (other.ActiveLayers == ActiveLayers) && (other.OverlayLayerRightHand == OverlayLayerRightHand) && (other.OverlayLayerLeftHand == OverlayLayerLeftHand) && (other.OverlayLayerBothHands == OverlayLayerBothHands);
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