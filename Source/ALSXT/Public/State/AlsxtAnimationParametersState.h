#pragma once

#include "GameplayTagContainer.h"
#include "Utility/AlsxtGameplayTags.h"
#include "Utility/AlsxtOverlayStructs.h"
#include "State/AlsxtBreathState.h"
#include "AlsxtAnimationParametersState.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtOverlayModesState
{
	GENERATED_BODY()
	
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtAnimationLayerState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (Categories = "Als.Locomotion Variant, Als.Status Effect Locomotion Variant, Als.Overlay", AllowPrivateAccess))
	FGameplayTag Layer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 1))
	float Alpha{ 0.0f };

	bool operator==(const FAlsxtAnimationLayerState& other) const
	{
		return (other.Layer == Layer) && (other.Alpha == Alpha);
	}

	bool IsValid() const
	{
		return Layer.IsValid() && Alpha > 0.0f;
	}

	explicit operator bool() const
	{
		return IsValid();
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtAnimationLayerStateRanked
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (AllowPrivateAccess))
	FAlsxtAnimationLayerState State;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 1))
	float Weight{ 1.0 };

	bool operator==(const FAlsxtAnimationLayerStateRanked& other) const
	{
		return (other.State == State) && (other.Weight == Weight);
	}

	bool IsValid() const
	{
		return State.Layer.IsValid();
	}

	explicit operator bool() const
	{
		return IsValid();
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtLocomotionBlendState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (AllowPrivateAccess))
	FGameplayTag BaseLocomotionVariant;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (AllowPrivateAccess))
	FAlsxtAnimationLayerState FullBodyLocomotionLayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (AllowPrivateAccess))
	FAlsxtAnimationLayerState RightSideLocomotionLayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (AllowPrivateAccess))
	FAlsxtAnimationLayerState LeftSideLocomotionLayer;
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtOverlayBlendState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (AllowPrivateAccess))
	FAlsxtAnimationLayerState FullBodyOverlayLayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (AllowPrivateAccess))
	FAlsxtAnimationLayerState RightSideOverlayLayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (AllowPrivateAccess))
	FAlsxtAnimationLayerState LeftSideOverlayLayer;
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtAnimationBlendState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (AllowPrivateAccess))
	FAlsxtLocomotionBlendState LocomotionBlendState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (AllowPrivateAccess))
	FAlsxtOverlayBlendState OverlayBlendState;
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtAnimationParametersState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (Categories = "Als.Sex,Als.Locomotion Variant", AllowPrivateAccess))
	FGameplayTagContainer CharacteristicsTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (AllowPrivateAccess))
	FAlsxtAnimationBlendState AnimationBlendState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Meta = (AllowPrivateAccess))
	FAlsxtBreathState BreathState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (Categories = "Als.Gesture,Als.Hand,Als.OverlayLeftHandMirrorPolicy", AllowPrivateAccess))
	FGameplayTagContainer Gesture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (Categories = "Als.Firearm Finger Pose,Als.Hand,Als.OverlayLeftHandMirrorPolicy", AllowPrivateAccess))
	FGameplayTag TriggerFingerPose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (Categories = "Als.Freelooking", AllowPrivateAccess))
	FGameplayTagContainer ViewTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (Categories = "Als.LocomotionMode,Als.Status Effect Locomotion Variant,Als.LocomotionModifier,Als.LocomotionModifierSettings", AllowPrivateAccess))
	FGameplayTagContainer LocomotionTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (Categories = "Als.OverlayMode,Als.Overlay Stance", AllowPrivateAccess))
	FGameplayTagContainer OverlayBothHands;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (Categories = "Als.OverlayMode,Als.Overlay Stance,Als.Status Effect Locomotion Variant,Als.OverlayLeftHandMirrorPolicy,Als.OverlaySettings", AllowPrivateAccess))
	FGameplayTagContainer OverlayLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (Categories = "Als.OverlayMode,Als.Overlay Stance,Als.Status Effect Locomotion Variant,Als.OverlayLeftHandMirrorPolicy,Als.OverlaySettings", AllowPrivateAccess))
	FGameplayTagContainer OverlayRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (Categories = "Als.OverlayMode,Als.Overlay Stance", AllowPrivateAccess))
	FGameplayTag AdditiveIdle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (Categories = "Als.OverlayMode,Als.OverlayLeftHandMirrorPolicy,Als.OverlaySettings", AllowPrivateAccess))
	FGameplayTagContainer OverlayTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (AllowPrivateAccess))
	FAlsxtOverlaySlots OverlaySlots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (Categories = "Als.Stance,Als.Combat Stance,Als.Ready Stance,Als.Weapon Firearm Stance,Als.Weapon Carry Position,Als.Weapon Ready Position", AllowPrivateAccess))
	FGameplayTagContainer StanceTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (Categories = "Als.Defensive Mode,Als.Injury,Als.Combat Stance", AllowPrivateAccess))
	FGameplayTagContainer PoseTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (Categories = "Als.Grip Position,Als.Foregrip Position", AllowPrivateAccess))
	FGameplayTagContainer GripTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (Categories = "Als.Stationary Mode,Als.Vehicle Seat", AllowPrivateAccess))
	FGameplayTagContainer StationaryModeTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (Categories = "Als.Stationary Mode,Als.Vehicle Seat", AllowPrivateAccess))
	FGameplayTagContainer StatusEffectLocomotionVariantSeverity;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 2))
	float BreathingRate{ 0.5 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 2))
	float BreathingAlpha{ 0.5 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 2))
	float DefensiveAmount{ 0.0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 2))
	float Exhaustion{ 0.0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 2))
	float InjuryUpperAmount{ 0.5 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 2))
	float InjuryLowerAmount{ 0.5 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ClampMin = 0, ClampMax = 1))
	float SeatedAmount {0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ClampMin = -1, ClampMax = 1))
	float LeanAmount {0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ClampMin = -1, ClampMax = 1))
	float CantedAmount {0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ClampMin = 0, ClampMax = 1))
	float VaultingAmount {0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ClampMin = 0, ClampMax = 1))
	float SlidingAmount {0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ClampMin = 0, ClampMax = 1))
	float WallJumpAmount {0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ClampMin = 0, ClampMax = 1))
	float WallRunAmount {0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ClampMin = 0, ClampMax = 1))
	float StationaryAmount {0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FVector DefensivePoint {FVector(0.0f, 0.0f, 0.0f)};	

	bool operator==(const FAlsxtAnimationParametersState& other) const
	{
		return (other.CharacteristicsTags == CharacteristicsTags) && (other.OverlayTags == OverlayTags) && (other.StanceTags == StanceTags) && (other.PoseTags == PoseTags) && (other.GripTags == GripTags) && (other.StationaryModeTags == StationaryModeTags);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtOverlayObjectAnimationParametersState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (Categories = "Als.Sex,Als.Locomotion Variant", AllowPrivateAccess))
	FGameplayTagContainer CharacteristicsTags;

	bool operator==(const FAlsxtOverlayObjectAnimationParametersState& other) const
	{
		return (other.CharacteristicsTags == CharacteristicsTags);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtAimableOverlayObjectAnimationParametersState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (Categories = "Als.Sex,Als.Locomotion Variant", AllowPrivateAccess))
	FGameplayTagContainer CharacteristicsTags;

	bool operator==(const FAlsxtAimableOverlayObjectAnimationParametersState& other) const
	{
		return (other.CharacteristicsTags == CharacteristicsTags);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtFirearmOverlayObjectAnimationParametersState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (Categories = "Als.Firearm Finger Pose,Als.Hand,Als.OverlayLeftHandMirrorPolicy", AllowPrivateAccess))
	FGameplayTag TriggerFingerPose;

	bool operator==(const FAlsxtFirearmOverlayObjectAnimationParametersState& other) const
	{
		return (other.TriggerFingerPose == TriggerFingerPose);
	}
};