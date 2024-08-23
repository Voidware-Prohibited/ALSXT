#pragma once

#include "Utility/ALSXTStructs.h"
#include "Engine/DataAsset.h"
#include "Engine/EngineTypes.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimSequenceBase.h"
#include "ALSXTStationaryModeSettings.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTStationaryModeState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	TObjectPtr<UAnimSequenceBase> Pose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	TWeakObjectPtr<UPrimitiveComponent> TargetPrimitive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Impact Side", AllowPrivateAccess))
	FGameplayTag Mode;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FVector MountPoint{ ForceInit };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Impact Side", AllowPrivateAccess))
	FGameplayTagContainer VehicleSide;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Impact Side", AllowPrivateAccess))
	FGameplayTag VehicleWindowSide;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FVector2D AimYawRange{ -180.0f, 180.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FVector2D AimPitchRange{ -180.0f, 180.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FVector2D CameraYawRange{ -180.0f, 180.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FVector2D CameraPitchRange{ -180.0f, 180.0f };

	bool operator==(const FALSXTStationaryModeState& other) const
	{
		return (other.Pose == Pose) && (other.TargetPrimitive == TargetPrimitive) && (other.Mode == Mode) && (other.MountPoint == MountPoint) && (other.VehicleSide == VehicleSide) && (other.VehicleWindowSide == VehicleWindowSide);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTStationaryModeParameters
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	TWeakObjectPtr<UPrimitiveComponent> TargetPrimitive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FVector_NetQuantize100 TargetRelativeLocation{ForceInit};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FRotator TargetRelativeRotation{ForceInit};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ForceUnits = "cm"))
	float ImpactHeight{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FGameplayTag Location{FGameplayTag::EmptyTag};
};

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UALSXTStationaryModeSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{StaticStationaryMode}", AllowPrivateAccess))
	TArray<FStaticStationaryModeAnimation> StaticStationaryModeAnimations;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FVector2D FirstPersonYawRange {-180.0f, 180.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FVector2D FirstPersonPitchRange {-180.0f, 180.0f};

	// Mantling time to blend in amount curve.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UCurveFloat> BlendInCurve;

	// Mantling time to interpolation, horizontal and vertical correction amounts curve.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UCurveVector> InterpolationAndCorrectionAmountsCurve;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FVector StartRelativeLocation{-65.0f, 0.0f, -100.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ClampMin = 0))
	FVector2D ReferenceHeight{50.0f, 100.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ClampMin = 0))
	FVector2D StartTime{0.5f, 0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ClampMin = 0))
	FVector2D PlayRate{1.0f, 1.0f};

public:
	float CalculateStartTime(float UnarmedAttackHeight) const;

	float CalculatePlayRate(float UnarmedAttackHeight) const;
};

inline float UALSXTStationaryModeSettings::CalculateStartTime(const float UnarmedAttackHeight) const
{
	return FMath::GetMappedRangeValueClamped(ReferenceHeight, StartTime, UnarmedAttackHeight);
}

inline float UALSXTStationaryModeSettings::CalculatePlayRate(const float UnarmedAttackHeight) const
{
	return FMath::GetMappedRangeValueClamped(ReferenceHeight, PlayRate, UnarmedAttackHeight);
}

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTGeneralStationaryModeSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAllowStationaryMode{ true };
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRotateToInputOnStart{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0))
	float RotationInterpolationSpeed{ 10.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = -180, ClampMax = 180))
	float RotationOffset{ 45.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	TArray<TEnumAsByte<EObjectTypeQuery>> AttackTraceObjectTypes;

};