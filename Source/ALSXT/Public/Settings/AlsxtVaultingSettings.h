#pragma once

#include "Engine/DataAsset.h"
#include "Engine/EngineTypes.h"
#include "Utility/AlsxtStructs.h"
#include "Settings/AlsMantlingSettings.h"
#include "AlsxtVaultingSettings.generated.h"

class UAnimMontage;
class UCurveFloat;
class UCurveVector;

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtVaultingParameters
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	TWeakObjectPtr<UPrimitiveComponent> TargetPrimitive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FVector_NetQuantize100 TargetHandPlantRelativeLocation{ForceInit};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FVector_NetQuantize100 TargetRelativeLocation{ForceInit};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FRotator TargetRelativeRotation{ForceInit};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ForceUnits = "cm"))
	float VaultingHeight{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FGameplayTag LocomotionMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FGameplayTag Gait;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FGameplayTag VaultingType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FVaultAnimation VaultAnimation;
};

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UAlsxtVaultingSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TArray<FVaultAnimation> VaultAnimations;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UAnimMontage> Montage;

	// If checked, mantling will automatically calculate the start time based on how much vertical
	// distance the character needs to move to reach the object they are about to mantle.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ClampMin = 0))
	uint8 bAutoCalculateStartTime : 1 {false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ClampMin = 0, EditCondition = "!bAutoCalculateStartTime"))
	FVector2f StartTimeReferenceHeight{50.0f, 100.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ClampMin = 0, EditCondition = "!bAutoCalculateStartTime"))
	FVector2f StartTime{0.5f, 0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ClampMin = 0, ForceUnits = "t"))
	FFloatInterval MotionWarpingTimeRange{0.0f, 0.3f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	EAlphaBlendOption MotionWarpingLocationBlendOption{EAlphaBlendOption::Linear};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings",
		Meta = (EditCondition = "MotionWarpingLocationBlendOption == EAlphaBlendOption::Custom", EditConditionHides))
	TObjectPtr<UCurveFloat> MotionWarpingLocationCustomBlendCurve;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	EAlphaBlendOption MotionWarpingRotationBlendOption{EAlphaBlendOption::HermiteCubic};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings",
		Meta = (EditCondition = "MotionWarpingRotationBlendOption == EAlphaBlendOption::Custom", EditConditionHides))
	TObjectPtr<UCurveFloat> MotionWarpingRotationCustomBlendCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float VerticalOffset {0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FVector2D FirstPersonYawRange {-180.0f, 180.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FVector2D FirstPersonPitchRange {-180.0f, 180.0f};

public:
	float GetStartTimeForHeight(FVector2D ReferenceHeight, FVector2D InStartTime, float MantlingHeight) const;

	float GetPlayRateForHeight(FVector2D ReferenceHeight, FVector2D PlayRate, float MantlingHeight) const;
};

inline float UAlsxtVaultingSettings::GetStartTimeForHeight(const FVector2D ReferenceHeight, const FVector2D InStartTime, const float MantlingHeight) const
{
	return FMath::GetMappedRangeValueClamped(ReferenceHeight, InStartTime, MantlingHeight);
}

inline float UAlsxtVaultingSettings::GetPlayRateForHeight(const FVector2D ReferenceHeight, const FVector2D PlayRate, const float MantlingHeight) const
{
	return FMath::GetMappedRangeValueClamped(ReferenceHeight, PlayRate, MantlingHeight);
}

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtVaultingTraceSettings
{
	GENERATED_BODY()

	// Height Range of Obstacle to be Vaultable
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ClampMin = 0))
	FVector2D LedgeHeight{50.0f, 225.0f};

	// Minimum Reach Distance for Trace. This is Multiplied by the Characters Velocity and ReachDistanceVelocityMultiplier
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ClampMin = 0, ForceUnits = "cm"))
	float BaseReachDistance{78.0f};

	// Value used to determine Reach Distance in proportion to Character Velocity
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ClampMin = 0))
	float ReachDistanceVelocityMultiplier{ 0.0075f };

	// Initial Distance away from Obstacle when landing to prevent clipping with the Obstacle
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ClampMin = 0, ForceUnits = "cm"))
	float TargetLocationOffset{15.0f};

	// Maximum Depth of Obstacle to be Vaultable
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ClampMin = 0, ForceUnits = "cm"))
	float MaxDepth{60.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ClampMin = 0))
	bool bDrawFailedTraces{false};
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtGeneralVaultingSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actions|Vaulting")
	bool bAllowVaulting{true};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actions|Vaulting|Obstacle Trace", Meta = (ClampMin = 0, ClampMax = 180, ForceUnits = "deg"))
	float TraceAngleThreshold{110.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actions|Vaulting|Obstacle Trace", Meta = (ClampMin = 0, ClampMax = 180, ForceUnits = "deg"))
	float MaxReachAngle{50.0f};

	// Distance away from Obstacle being vaulted that the Character lands in Walking Gait
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actions|Vaulting|Landing", Meta = (ForceUnits = "cm"))
	float WalkVaultingForwardDistance { 50.0f };

	// Distance away from Obstacle being vaulted that the Character lands in Running Gait
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actions|Vaulting|Landing", Meta = (ForceUnits = "cm"))
	float RunVaultingForwardDistance { 90.0f };

	// Distance away from Obstacle being vaulted that the Character lands in Sprinting Gait
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actions|Vaulting|Landing", Meta = (ForceUnits = "cm"))
	float SprintVaultingForwardDistance { 175.0f };

	// If a dynamic object has a speed bigger than this value, then do not start mantling.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actions|Vaulting|Obstacle Trace", Meta = (ForceUnits = "cm/s"))
	float TargetPrimitiveSpeedThreshold{10.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actions|Vaulting|Obstacle Trace", Meta = (ForceUnits = "cm"))
	float VaultingHighHeightThreshold{125.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actions|Vaulting|Obstacle Trace")
	FAlsxtVaultingTraceSettings GroundedTrace;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actions|Vaulting|Obstacle Trace")
	FAlsxtVaultingTraceSettings InAirTrace{{50.0f, 150.0f}, 70.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actions|Vaulting|Obstacle Trace")
	TArray<TEnumAsByte<EObjectTypeQuery>> VaultingTraceObjectTypes;
};
