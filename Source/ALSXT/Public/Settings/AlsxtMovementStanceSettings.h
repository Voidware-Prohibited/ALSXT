#pragma once

#include "Engine/DataAsset.h"
#include "Utility/AlsGameplayTags.h"
#include "Utility/AlsxtGameplayTags.h"
#include "AlsxtMovementStanceSettings.generated.h"

struct FAlsMovementStanceSettings;
class UCurveFloat;
class UCurveVector;

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtMovementGaitSettings
{
	GENERATED_BODY()

public:
	
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
class ALSXT_API UAlsxtMovementStanceSettingsAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ForceInlineRow))
	FAlsxtMovementStanceSettings StanceSettings;

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