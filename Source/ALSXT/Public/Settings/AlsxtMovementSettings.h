#pragma once

#include "AlsxtMovementStanceSettings.h"
#include "AlsxtOverlayLookupTableDataAsset.h"
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
struct ALSXT_API FAlsxtMovementSlopeSlidingSettings
{
	GENERATED_BODY()

	// When enabled, MaxWalkableSlopeAngle will be set to FallAngle, and Character will slide when traversing between SlideSlopeAngle and 60 degrees.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slope Sliding")
	bool bEnableSlopeSliding{true};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slope Sliding", Meta = (EditCondition="bEnableSlopeSliding", ClampMin = 0, ForceUnits = "cm/s"))
	float SlopeSlideMovementSpeed{500.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slope Sliding", Meta = (EditCondition="bEnableSlopeSliding", ClampMin = 0.0f, ClampMax = 100.0))
	float Priority{100.0f};	

	// Default is 40.0
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slope Sliding|Advanced", Meta = (EditCondition="bEnableSlopeSliding", ClampMin = 35.0f, ClampMax = 60.0, ForceUnits = "Degrees"))
	float SlopeSlideAngle{40.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slope Sliding|Advanced", Meta = (EditCondition="bEnableSlopeSliding", ClampMin = 44.0f, ClampMax = 89.0, ForceUnits = "Degrees"))
	float FallAngle{60.0f};

	// The amount of influence surface Wetness has on sliding
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slope Sliding|Advanced", Meta = (EditCondition="bEnableSlopeSliding", ClampMin = 1.0f, ClampMax = 10.0))
	float WetnessSlideSlopeAngleModifier{3.0f};

	// The amount of influence that the Character weight has on sliding. If surface is Mud-like it will resist sliding, but if the surface is smooth it will increase sliding.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slope Sliding|Advanced", Meta = (EditCondition="bEnableSlopeSliding", ClampMin = 1.0f, ClampMax = 10.0))
	float SlopeSlidingWeightResistanceFactor{1.0f};
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtMovementPhysicalMaterialSettings
{
	GENERATED_BODY()

	// Currently, the direction-dependent movement speed can cause some jitter in multiplayer, so enable it at your own risk.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ALS")
	uint8 bAllowDirectionDependentMovementSpeed : 1 {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "", Meta = (ClampMin = 0, ForceUnits = "cm/s"))
	float ForwardMovementSpeed{500.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "", Meta = (EditCondition="bAllowDirectionDependentMovementSpeed", ClampMin = 0, ForceUnits = "cm/s"))
	float BackwardMovementSpeed{500.0f};

	// When enabled, MaxWalkableSlopeAngle will be set to FallAngle, and Character will slide when traversing between SlideSlopeAngle and 60 degrees.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slope Sliding")
	FAlsxtMovementSlopeSlidingSettings SprintSlopeSlidingSettings;
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtMovementGaitsSurfaceSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ForceInlineRow))
	TMap<UPARAM(meta = (Categories = "Als.Gait"))FGameplayTag, FAlsxtMovementPhysicalMaterialSettings> Gaits
	{
					{AlsGaitTags::Walking, {}},
					{AlsGaitTags::Combat, {}},
					{AlsGaitTags::Running, {}}
	};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "", Meta = (ClampMin = 0, ForceUnits = "cm/s"))
	float SprintSpeed{650.0f};

	// When enabled, MaxWalkableSlopeAngle will be set to FallAngle, and Character will slide when traversing between SlideSlopeAngle and 60 degrees.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slope Sliding")
	FAlsxtMovementSlopeSlidingSettings SprintSlopeSlidingSettings;

	// Gait amount to acceleration, deceleration, and ground friction curve.
	// Gait amount ranges from 0 to 3, where 0 is stopped, 1 is walking, 2 is running, and 3 is sprinting.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	TSoftObjectPtr<UCurveVector> AccelerationAndDecelerationAndGroundFrictionCurve;

	// Gait amount to rotation interpolation speed curve.
	// Gait amount ranges from 0 to 3, where 0 is stopped, 1 is walking, 2 is running, and 3 is sprinting.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	TSoftObjectPtr<UCurveFloat> RotationInterpolationSpeedCurve;	
	
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtMovementStanceSurfaceSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ForceInlineRow))
	TMap<UPARAM(meta = (Categories = "Als.Stance"))FGameplayTag, FAlsxtMovementGaitsSurfaceSettings> Stances
	{
		{AlsStanceTags::Standing, {}},
		{AlsStanceTags::Crouching, {}}
	};
	
};

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UAlsxtMovementStanceSurfaceSettingsDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Features|Slope Sliding", Meta = (ClampMin = 0.0f, ClampMax = 100.0))
	float Weight {0.0f};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ForceInlineRow))
	FAlsxtMovementStanceSurfaceSettings Stances;
	
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtMovementSurfaceSettings
{
	GENERATED_BODY()

	// If the current Surface is not defined in AdditionalSurfaces, these settings will be used.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ForceInlineRow))
	TSoftObjectPtr<UAlsxtMovementStanceSurfaceSettingsDataAsset> DefaultSurface;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ForceInlineRow))
	TMap<TSoftObjectPtr<UPhysicalMaterial>, TSoftObjectPtr<UAlsxtMovementStanceSurfaceSettingsDataAsset>> AdditionalSurfaces;
	
};

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UAlsxtMovementSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	// If settings for Surface is not defined, 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement Speed", Meta = (ForceInlineRow))
	TSoftObjectPtr<UPhysicalMaterial> DefaultPhysicalMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement Speed", Meta = (ForceInlineRow))
	FAlsxtMovementSurfaceSettings GroundedLocomotionMode;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement Speed", Meta = (ForceInlineRow))
	TMap<FGameplayTag, FAlsxtMovementRotationModeSettings> AdditionalLocomotionModes
	{
			{AlsLocomotionModeTags::Ladder, {}},
			{AlsLocomotionModeTags::Climbing, {}},
			{AlsLocomotionModeTags::SwimmingTreading, {}},
			{AlsLocomotionModeTags::SwimmingUnderwater, {}}
	};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ForceInlineRow))
	TMap<FGameplayTag, FAlsxtStatusLocomotionModeSettings>  StatusLocomotionModes;

	// Range of velocity angle relative to the view direction at which
	// interpolation from forward speed to backward speed will take place.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interpolation", Meta = (ClampMin = 0, ClampMax = 180, ForceUnits = "deg"))
	FVector2f VelocityAngleToSpeedInterpolationRange{100.0f, 125.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Effects")
	bool bCanClimbWithBrokenArm{false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Effects")
	bool bCanClimbAndAimWithBrokenArm{false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Effects")
	bool bCanClimbWithBrokenLeg{false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Effects")
	bool bCanSwimWithBrokenLeg{false};

	// When enabled, MaxWalkableSlopeAngle will be set to FallAngle, and Character will slide when traversing between SlideSlopeAngle and 60 degrees.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slope Sliding")
	bool bEnableSlopeSliding{true};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slope Sliding", Meta = (EditCondition="bEnableSlopeSliding", ClampMin = 35.0f, ClampMax = 60.0, ForceUnits = "Degrees"))
	float BaseSlopeSlideAngle{40.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slope Sliding")
	bool UseSurfaceFrictionForSlopeSliding{true};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slope Sliding")
	bool UseCharacterWeightForSlopeSliding{true};

public:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& ChangedEvent) override;
#endif
};