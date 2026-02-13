#pragma once

#include "GameplayTagContainer.h"
#include "Utility/AlsxtGameplayTags.h"
#include "Utility/AlsxtOverlayStructs.h"
#include "AlsxtAnimationParametersState.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtOverlayModesState
{
	GENERATED_BODY()
	
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtStatusEffectLocomotionVariantStateEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (Categories = "Als.Status Effect Locomotion Variant", AllowPrivateAccess))
	FGameplayTag StatusEffectLocomotionVariant;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 1))
	float Alpha{ 0.5 };
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtStatusEffectLocomotionVariantStateRankedEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (AllowPrivateAccess))
	FAlsxtStatusEffectLocomotionVariantStateEntry Entry;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 1))
	float Weight{ 1.0 };
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtAnimationParametersState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (Categories = "Als.Sex,Als.Locomotion Variant", AllowPrivateAccess))
	FGameplayTagContainer CharacteristicsTags;

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