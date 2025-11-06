#pragma once

#include "Engine/DataAsset.h"
#include "Utility/AlsGameplayTags.h"
#include "Utility/AlsxtGameplayTags.h"
#include "AlsxtMovementSettings.generated.h"

struct FAlsMovementStanceSettings;
class UCurveFloat;
class UCurveVector;

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtMovementGaitSettings
{
	GENERATED_BODY()

public:
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ClampMin = 0, ForceUnits = "cm/s"))
	float ClimbingWalkForwardSpeed{175.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS",
		Meta = (ClampMin = 0, EditCondition = "bAllowDirectionDependentMovementSpeed", ForceUnits = "cm/s"))
	float ClimbingWalkBackwardSpeed{175.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ClampMin = 0, ForceUnits = "cm/s"))
	float ClimbingAimingForwardSpeed{175.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS",
		Meta = (ClampMin = 0, EditCondition = "bAllowDirectionDependentMovementSpeed", ForceUnits = "cm/s"))
	float ClimbingAimingBackwardSpeed{175.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ClampMin = 0, ForceUnits = "cm/s"))
	float ClimbingCombatForwardSpeed{175.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS",
		Meta = (ClampMin = 0, EditCondition = "bAllowDirectionDependentMovementSpeed", ForceUnits = "cm/s"))
	float ClimbingCombatBackwardSpeed{175.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ClampMin = 0, ForceUnits = "cm/s"))
	float ClimbingRunForwardSpeed{375.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS",
		Meta = (ClampMin = 0, EditCondition = "bAllowDirectionDependentMovementSpeed", ForceUnits = "cm/s"))
	float ClimbingRunBackwardSpeed{375.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ClampMin = 0, ForceUnits = "cm/s"))
	float ClimbingSprintSpeed{650.0f};

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
	float GetMaxWalkSpeed() const;
	float GetMaxAimingSpeed() const;
	float GetMaxCombatSpeed() const;
	float GetMaxRunSpeed() const;
	float GetMaxSprintSpeed() const;

	float GetMaxClimbingWalkSpeed() const;
	float GetMaxClimbingAimingSpeed() const;
	float GetMaxClimbingCombatSpeed() const;
	float GetMaxClimbingRunSpeed() const;
	float GetMaxClimbingSprintSpeed() const;

	float GetMaxSwimmingWalkSpeed() const;
	float GetMaxSwimmingAimingSpeed() const;
	float GetMaxSwimmingCombatSpeed() const;
	float GetMaxSwimmingRunSpeed() const;
	float GetMaxSwimmingSprintSpeed() const;
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtMovementStanceSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ForceInlineRow))
	TMap<FGameplayTag, FAlsxtMovementGaitSettings> Stances
	{
		{AlsStanceTags::Standing, {}},
		{AlsStanceTags::Crouching, {}},
		{AlsStanceTags::Prone, {}}
		
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

public:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& ChangedEvent) override;
#endif
};

inline float FAlsxtMovementGaitSettings::GetMaxWalkSpeed() const
{
	return bAllowDirectionDependentMovementSpeed
			   ? FMath::Max(WalkForwardSpeed, WalkBackwardSpeed)
			   : WalkForwardSpeed;
}

inline float FAlsxtMovementGaitSettings::GetMaxAimingSpeed() const
{
	return bAllowDirectionDependentMovementSpeed
			   ? FMath::Max(AimingForwardSpeed, AimingBackwardSpeed)
			   : AimingForwardSpeed;
}

inline float FAlsxtMovementGaitSettings::GetMaxCombatSpeed() const
{
	return bAllowDirectionDependentMovementSpeed
			   ? FMath::Max(CombatForwardSpeed, CombatBackwardSpeed)
			   : CombatForwardSpeed;
}

inline float FAlsxtMovementGaitSettings::GetMaxRunSpeed() const
{
	return bAllowDirectionDependentMovementSpeed
			   ? FMath::Max(WalkForwardSpeed, WalkBackwardSpeed)
			   : WalkForwardSpeed;
}

inline float FAlsxtMovementGaitSettings::GetMaxSprintSpeed() const
{
	return bAllowDirectionDependentMovementSpeed
			   ? FMath::Max(WalkForwardSpeed, WalkBackwardSpeed)
			   : WalkForwardSpeed;
}

inline float FAlsxtMovementGaitSettings::GetMaxClimbingWalkSpeed() const
{
	return bAllowDirectionDependentMovementSpeed
			   ? FMath::Max(ClimbingWalkForwardSpeed, ClimbingWalkBackwardSpeed)
			   : ClimbingWalkForwardSpeed;
}

inline float FAlsxtMovementGaitSettings::GetMaxClimbingAimingSpeed() const
{
	return bAllowDirectionDependentMovementSpeed
			   ? FMath::Max(ClimbingAimingForwardSpeed, ClimbingAimingBackwardSpeed)
			   : ClimbingAimingForwardSpeed;
}

inline float FAlsxtMovementGaitSettings::GetMaxClimbingCombatSpeed() const
{
	return bAllowDirectionDependentMovementSpeed
			   ? FMath::Max(ClimbingCombatForwardSpeed, ClimbingCombatBackwardSpeed)
			   : ClimbingCombatForwardSpeed;
}

inline float FAlsxtMovementGaitSettings::GetMaxClimbingRunSpeed() const
{
	return bAllowDirectionDependentMovementSpeed
			   ? FMath::Max(ClimbingWalkForwardSpeed, ClimbingWalkBackwardSpeed)
			   : ClimbingWalkForwardSpeed;
}

inline float FAlsxtMovementGaitSettings::GetMaxClimbingSprintSpeed() const
{
	return bAllowDirectionDependentMovementSpeed
			   ? FMath::Max(ClimbingWalkForwardSpeed, ClimbingWalkBackwardSpeed)
			   : ClimbingWalkForwardSpeed;
}

//

inline float FAlsxtMovementGaitSettings::GetMaxSwimmingWalkSpeed() const
{
	return bAllowDirectionDependentMovementSpeed
			   ? FMath::Max(SwimmingWalkForwardSpeed, SwimmingWalkBackwardSpeed)
			   : SwimmingWalkForwardSpeed;
}

inline float FAlsxtMovementGaitSettings::GetMaxSwimmingAimingSpeed() const
{
	return bAllowDirectionDependentMovementSpeed
			   ? FMath::Max(SwimmingAimingForwardSpeed, SwimmingAimingBackwardSpeed)
			   : SwimmingAimingForwardSpeed;
}

inline float FAlsxtMovementGaitSettings::GetMaxSwimmingCombatSpeed() const
{
	return bAllowDirectionDependentMovementSpeed
			   ? FMath::Max(SwimmingCombatForwardSpeed, SwimmingCombatBackwardSpeed)
			   : SwimmingCombatForwardSpeed;
}

inline float FAlsxtMovementGaitSettings::GetMaxSwimmingRunSpeed() const
{
	return bAllowDirectionDependentMovementSpeed
			   ? FMath::Max(SwimmingWalkForwardSpeed, SwimmingWalkBackwardSpeed)
			   : SwimmingWalkForwardSpeed;
}

inline float FAlsxtMovementGaitSettings::GetMaxSwimmingSprintSpeed() const
{
	return bAllowDirectionDependentMovementSpeed
			   ? FMath::Max(SwimmingWalkForwardSpeed, SwimmingWalkBackwardSpeed)
			   : SwimmingWalkForwardSpeed;
}