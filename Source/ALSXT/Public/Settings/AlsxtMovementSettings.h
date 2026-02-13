#pragma once

#include "AlsxtMovementStanceSettings.h"
#include "Engine/DataAsset.h"
#include "Utility/AlsGameplayTags.h"
#include "Utility/AlsxtGameplayTags.h"
#include "AlsxtMovementSettings.generated.h"

struct FAlsMovementStanceSettings;
class UCurveFloat;
class UCurveVector;

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtMovementRotationModeSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ForceInlineRow))
	TMap<FGameplayTag, TSubclassOf<UAlsxtMovementStanceSettingsAsset>> RotationModes
	{
					{AlsRotationModeTags::VelocityDirection, {}},
					{AlsRotationModeTags::ViewDirection, {}},
					{AlsRotationModeTags::Aiming, {}}
	};
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtStatusLocomotionModeSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ForceInlineRow, Categories="Als.StatusLocomotionVariant"))
	TMap<FGameplayTag, FAlsxtMovementRotationModeSettings> RotationModes
	{
						{AlsxtStatusEffectLocomotionVariantSeverityTags::Light, {}},
						{AlsxtStatusEffectLocomotionVariantSeverityTags::Moderate, {}},
						{AlsxtStatusEffectLocomotionVariantSeverityTags::Severe, {}}
	};
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtMovementPhysicalMaterialSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Features|Slope Sliding", Meta = (EditCondition="bEnableSlopeSliding", ClampMin = 0.0f, ClampMax = 100.0))
	float Priority{100.0f};

	// When enabled, MaxWalkableSlopeAngle will be set to FallAngle, and Character will slide when traversing between SlideSlopeAngle and 60 degrees.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Features|Slope Sliding")
	bool bEnableSlopeSliding{true};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Features|Slope Sliding", Meta = (EditCondition="bEnableSlopeSliding", ClampMin = 35.0f, ClampMax = 60.0, ForceUnits = "Degrees"))
	float SlopeSlideAngle{40.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Features|Slope Sliding", Meta = (EditCondition="bEnableSlopeSliding", ClampMin = 44.0f, ClampMax = 89.0, ForceUnits = "Degrees"))
	float FallAngle{60.0f};

	// The amount of influence surface Wetness has on sliding
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Features|Slope Sliding", Meta = (EditCondition="bEnableSlopeSliding", ClampMin = 1.0f, ClampMax = 10.0))
	float WetnessSlideSlopeAngleModifier{3.0f};

	// The amount of influence that the Character weight has on sliding. If surface is Mud-like it will resist sliding, but if the surface is smooth it will increase sliding.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Features|Slope Sliding", Meta = (EditCondition="bEnableSlopeSliding", ClampMin = 1.0f, ClampMax = 10.0))
	float SlopeSlidingWeightResistanceFactor{1.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ForceInlineRow))
	TMap<FGameplayTag, TSubclassOf<UAlsxtMovementStanceSettingsAsset>> RotationModes
	{
				{AlsRotationModeTags::VelocityDirection, {}},
				{AlsRotationModeTags::ViewDirection, {}},
				{AlsRotationModeTags::Aiming, {}}
	};
};

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UAlsxtMovementSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	// Range of velocity angle relative to the view direction at which
	// interpolation from forward speed to backward speed will take place.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ClampMin = 0, ClampMax = 180, ForceUnits = "deg"))
	FVector2f VelocityAngleToSpeedInterpolationRange{100.0f, 125.0f};

	// When enabled, MaxWalkableSlopeAngle will be set to FallAngle, and Character will slide when traversing between SlideSlopeAngle and 60 degrees.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Features|Slope Sliding")
	bool bEnableSlopeSliding{true};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Features|Slope Sliding", Meta = (EditCondition="bEnableSlopeSliding", ClampMin = 35.0f, ClampMax = 60.0, ForceUnits = "Degrees"))
	float BaseSlopeSlideAngle{40.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Features|Slope Sliding")
	bool UseSurfaceFrictionForSlopeSliding{true};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Features|Slope Sliding")
	bool UseCharacterWeightForSlopeSliding{true};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ForceInlineRow))
	TMap<FGameplayTag, FAlsxtMovementStanceSettings> RotationModes
	{
		{AlsRotationModeTags::VelocityDirection, {}},
		{AlsRotationModeTags::ViewDirection, {}},
		{AlsRotationModeTags::Aiming, {}}
	};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ForceInlineRow))
	TMap<TSubclassOf<UPhysicalMaterial>, FAlsxtMovementPhysicalMaterialSettings> GroundedMovement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ForceInlineRow))
	TMap<FGameplayTag, FAlsxtMovementRotationModeSettings> CustomLocomotionModes
	{
			{AlsLocomotionModeTags::Ladder, {}},
			{AlsLocomotionModeTags::Climbing, {}},
			{AlsLocomotionModeTags::SwimmingTreading, {}},
			{AlsLocomotionModeTags::SwimmingUnderwater, {}}
	};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ForceInlineRow))
	TMap<FGameplayTag, FAlsxtStatusLocomotionModeSettings>  StatusLocomotionModes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Features|Slope Sliding")
	bool bCanClimbWithBrokenArm{false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Features|Slope Sliding")
	bool bCanClimbAndAimWithBrokenArm{false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Features|Slope Sliding")
	bool bCanClimbWithBrokenLeg{false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Features|Slope Sliding")
	bool bCanSwimWithBrokenLeg{false};

public:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& ChangedEvent) override;
#endif
};