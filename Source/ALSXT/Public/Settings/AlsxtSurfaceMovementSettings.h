#pragma once

#include "Engine/DataAsset.h"
#include "Utility/AlsGameplayTags.h"
#include "Utility/AlsxtGameplayTags.h"
#include "GameplayTagContainer.h"
#include "AlsxtSurfaceMovementSettings.generated.h"


struct FAlsMovementStanceSettings;
class UCurveFloat;
class UCurveVector;

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtSurfaceMovementGaitSettings
{
	GENERATED_BODY()

public:
	// When enabled, MaxWalkableSlopeAngle will be set to FallAngle, and Character will slide when traversing between SlideSlopeAngle and 60 degrees.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Features|Slope Sliding")
	bool bEnableSlopeSliding{true};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Features|Slope Sliding", Meta = (EditCondition="bEnableSlopeSliding", ClampMin = 35.0f, ClampMax = 60.0, ForceUnits = "Degrees"))
	float SlopeSlideAngle{40.0f};

	// The amount of influence that the Character weight has on sliding. If surface is Mud-like it will resist sliding, but if the surface is smooth it will increase sliding.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Features|Slope Sliding", Meta = (EditCondition="bEnableSlopeSliding", ClampMin = 1.0f, ClampMax = 10.0))
	float SlopeSlidingWeightResistanceFactor{1.0f};
	
	// Currently, the direction-dependent movement speed can cause some jitter in multiplayer, so enable it at your own risk.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Features|Movement")
	uint8 bAllowDirectionDependentMovementSpeed : 1 {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ClampMin = 0, ForceUnits = "cm/s"))
	float WalkForwardSpeed{175.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS",
		Meta = (ClampMin = 0, EditCondition = "bAllowDirectionDependentMovementSpeed", ForceUnits = "cm/s"))
	float WalkBackwardSpeed{175.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ClampMin = 0, ForceUnits = "cm/s"))
	float AimingForwardSpeed{175.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS",
		Meta = (ClampMin = 0, EditCondition = "bAllowDirectionDependentMovementSpeed", ForceUnits = "cm/s"))
	float AimingBackwardSpeed{175.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ClampMin = 0, ForceUnits = "cm/s"))
	float CombatForwardSpeed{175.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS",
		Meta = (ClampMin = 0, EditCondition = "bAllowDirectionDependentMovementSpeed", ForceUnits = "cm/s"))
	float CombatBackwardSpeed{175.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ClampMin = 0, ForceUnits = "cm/s"))
	float RunForwardSpeed{375.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS",
		Meta = (ClampMin = 0, EditCondition = "bAllowDirectionDependentMovementSpeed", ForceUnits = "cm/s"))
	float RunBackwardSpeed{375.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ClampMin = 0, ForceUnits = "cm/s"))
	float SprintSpeed{650.0f};
	
	//

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ClampMin = 0, ForceUnits = "cm/s"))
	float SwimmingWalkForwardSpeed{175.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS",
		Meta = (ClampMin = 0, EditCondition = "bAllowDirectionDependentMovementSpeed", ForceUnits = "cm/s"))
	float SwimmingWalkBackwardSpeed{175.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ClampMin = 0, ForceUnits = "cm/s"))
	float SwimmingAimingForwardSpeed{175.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS",
		Meta = (ClampMin = 0, EditCondition = "bAllowDirectionDependentMovementSpeed", ForceUnits = "cm/s"))
	float SwimmingAimingBackwardSpeed{175.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ClampMin = 0, ForceUnits = "cm/s"))
	float SwimmingCombatForwardSpeed{175.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS",
		Meta = (ClampMin = 0, EditCondition = "bAllowDirectionDependentMovementSpeed", ForceUnits = "cm/s"))
	float SwimmingCombatBackwardSpeed{175.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ClampMin = 0, ForceUnits = "cm/s"))
	float SwimmingRunForwardSpeed{375.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS",
		Meta = (ClampMin = 0, EditCondition = "bAllowDirectionDependentMovementSpeed", ForceUnits = "cm/s"))
	float SwimmingRunBackwardSpeed{375.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ClampMin = 0, ForceUnits = "cm/s"))
	float SwimmingSprintSpeed{650.0f};

	// Gait amount to acceleration, deceleration, and ground friction curve.
	// Gait amount ranges from 0 to 3, where 0 is stopped, 1 is walking, 2 is running, and 3 is sprinting.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	TObjectPtr<UCurveVector> AccelerationAndDecelerationAndGroundFrictionCurve;

	// Gait amount to rotation interpolation speed curve.
	// Gait amount ranges from 0 to 3, where 0 is stopped, 1 is walking, 2 is running, and 3 is sprinting.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	TObjectPtr<UCurveFloat> RotationInterpolationSpeedCurve;

public:
	float GetMaxSurfaceWalkSpeed() const;
	float GetMaxSurfaceAimingSpeed() const;
	float GetMaxSurfaceCombatSpeed() const;
	float GetMaxSurfaceRunSpeed() const;
	float GetMaxSurfaceSprintSpeed() const;

	float GetMaxSurfaceSwimmingWalkSpeed() const;
	float GetMaxSurfaceSwimmingAimingSpeed() const;
	float GetMaxSurfaceSwimmingCombatSpeed() const;
	float GetMaxSurfaceSwimmingRunSpeed() const;
	float GetMaxSurfaceSwimmingSprintSpeed() const;
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtSurfaceMovementStanceSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ForceInlineRow))
	TMap<FGameplayTag, FAlsxtSurfaceMovementGaitSettings> Stances
	{
				{AlsStanceTags::Standing, {}},
				{AlsStanceTags::Crouching, {}},
				{AlsStanceTags::Prone, {}}
		
	};
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtSurfaceMovementSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ForceInlineRow))
	TMap<TSoftClassPtr<UPhysicalMaterial>, FAlsxtSurfaceMovementStanceSettings> Stances;
};

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UAlsxtSurfaceMovementSettingsAsset : public UDataAsset
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

	// When enabled, MaxWalkableSlopeAngle will be set to FallAngle, and Character will slide when traversing between SlideSlopeAngle and 60 degrees.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Features|Slope Sliding", Meta = (EditCondition="bEnableSlopeSliding", ClampMin = 35.0f, ClampMax = 60.0, ForceUnits = "Degrees"))
	float SlopeSlideAngle{40.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Features|Slope Sliding")
	bool UseSurfaceFrictionForSlopeSliding{true};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Features|Slope Sliding")
	bool UseCharacterWeightForSlopeSliding{true};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ForceInlineRow))
	TMap<FGameplayTag, FAlsxtSurfaceMovementGaitSettings> Stances
	{
			{AlsStanceTags::Standing, {}},
			{AlsStanceTags::Crouching, {}},
			{AlsStanceTags::Prone, {}}
		
	};

public:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& ChangedEvent) override;
#endif
};

inline float FAlsxtSurfaceMovementGaitSettings::GetMaxSurfaceWalkSpeed() const
{
	return bAllowDirectionDependentMovementSpeed
			   ? FMath::Max(WalkForwardSpeed, WalkBackwardSpeed)
			   : WalkForwardSpeed;
}

inline float FAlsxtSurfaceMovementGaitSettings::GetMaxSurfaceAimingSpeed() const
{
	return bAllowDirectionDependentMovementSpeed
			   ? FMath::Max(AimingForwardSpeed, AimingBackwardSpeed)
			   : AimingForwardSpeed;
}

inline float FAlsxtSurfaceMovementGaitSettings::GetMaxSurfaceCombatSpeed() const
{
	return bAllowDirectionDependentMovementSpeed
			   ? FMath::Max(CombatForwardSpeed, CombatBackwardSpeed)
			   : CombatForwardSpeed;
}

inline float FAlsxtSurfaceMovementGaitSettings::GetMaxSurfaceRunSpeed() const
{
	return bAllowDirectionDependentMovementSpeed
			   ? FMath::Max(WalkForwardSpeed, WalkBackwardSpeed)
			   : WalkForwardSpeed;
}

inline float FAlsxtSurfaceMovementGaitSettings::GetMaxSurfaceSprintSpeed() const
{
	return bAllowDirectionDependentMovementSpeed
			   ? FMath::Max(WalkForwardSpeed, WalkBackwardSpeed)
			   : WalkForwardSpeed;
}

//

inline float FAlsxtSurfaceMovementGaitSettings::GetMaxSurfaceSwimmingWalkSpeed() const
{
	return bAllowDirectionDependentMovementSpeed
			   ? FMath::Max(SwimmingWalkForwardSpeed, SwimmingWalkBackwardSpeed)
			   : SwimmingWalkForwardSpeed;
}

inline float FAlsxtSurfaceMovementGaitSettings::GetMaxSurfaceSwimmingAimingSpeed() const
{
	return bAllowDirectionDependentMovementSpeed
			   ? FMath::Max(SwimmingAimingForwardSpeed, SwimmingAimingBackwardSpeed)
			   : SwimmingAimingForwardSpeed;
}

inline float FAlsxtSurfaceMovementGaitSettings::GetMaxSurfaceSwimmingCombatSpeed() const
{
	return bAllowDirectionDependentMovementSpeed
			   ? FMath::Max(SwimmingCombatForwardSpeed, SwimmingCombatBackwardSpeed)
			   : SwimmingCombatForwardSpeed;
}

inline float FAlsxtSurfaceMovementGaitSettings::GetMaxSurfaceSwimmingRunSpeed() const
{
	return bAllowDirectionDependentMovementSpeed
			   ? FMath::Max(SwimmingWalkForwardSpeed, SwimmingWalkBackwardSpeed)
			   : SwimmingWalkForwardSpeed;
}

inline float FAlsxtSurfaceMovementGaitSettings::GetMaxSurfaceSwimmingSprintSpeed() const
{
	return bAllowDirectionDependentMovementSpeed
			   ? FMath::Max(SwimmingWalkForwardSpeed, SwimmingWalkBackwardSpeed)
			   : SwimmingWalkForwardSpeed;
}