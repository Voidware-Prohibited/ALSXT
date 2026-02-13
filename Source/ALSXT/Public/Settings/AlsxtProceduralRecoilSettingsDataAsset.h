// Copyright (C) 2026 Uriel Ballinas, VOIDWARE Prohibited. All rights reserved.
// This software is licensed under the MIT License (LICENSE.md).

#pragma once

#include "CoreMinimal.h"
#include "Curves/CurveVector.h"
#include "Engine/DataAsset.h"
#include "AlsxtProceduralRecoilSettingsDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FAlsxtProceduralRecoilSwaySettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Defaults)
	FVector2D PitchSwayRange{FVector2D::ZeroVector};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Defaults)
	FVector2D YawSwayRange{FVector2D::ZeroVector};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Defaults, meta = (ClampMin="0.0"))
	float RollSwayMultiplier{1.f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Defaults, meta = (ClampMin="0.0"))
	float Damping{0.f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Defaults, meta = (ClampMin="0.0"))
	float Acceleration{0.f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Defaults, meta = (ClampMin="0.0", ClampMax="1.0"))
	float AdsScale{1.f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Defaults)
	FVector PivotOffset{FVector::ZeroVector};
};

USTRUCT(BlueprintType)
struct FAlsxtProceduralRecoilAnimationInterpolationSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Defaults)
	float Acceleration = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Defaults)
	float Damping = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Defaults)
	float Amount = 0.f;
};

USTRUCT(BlueprintType)
struct FAlsxtProceduralRecoilControllerSettings
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Defaults)
	FVector2D HorizontalRecoilStep{FVector2D::ZeroVector};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Defaults)
	FVector2D VerticalRecoilStep{FVector2D::ZeroVector};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Defaults)
	FVector2D RecoilSmoothing{FVector2D::ZeroVector};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Defaults, meta = (ClampMin="0.0"))
	float Damping{0.f};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Defaults, meta = (ClampMin="0.0", ClampMax="1.0"))
	float Compensation{1.f};
};

UCLASS(BlueprintType)
class ALSXT_API UAlsxtProceduralRecoilSettingsDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	/** Saves recoil animation data by creating a data asset */
	void SaveData();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Controller Recoil")
	FAlsxtProceduralRecoilControllerSettings ControllerRecoil;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Controller Recoil")
	float BaseFireRate {0.0f};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Controller Recoil")
	int BurstFireModeRounds {3};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Advanced | Curve")
	UCurveVector* SingleFireRotationCurve = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Advanced | Curve")
	UCurveVector* SingleFireLocationCurve = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Advanced | Curve")
	UCurveVector* AutoFireRotationCurve = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Advanced | Curve")
	UCurveVector* AutoFireLocationCurve = nullptr;
	
	// Magnitudes for Pitch when aiming (X: min, Y: max).
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	FVector2D AimingPitchMagnitudes = FVector2D::ZeroVector;

	// Magnitudes for Yaw when aiming. X: min min, Y: min max,  Z: max min, W: max max
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	FVector4 AimingYawMagnitudes = FVector4(0, 0, 0, 0);

	/** Magnitudes for Roll when aiming
	 * X - min min
	 * Y - min max
	 * Z - max min
	 * W - max max
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	FVector4 AimingRollMagnitudes = FVector4(0, 0, 0, 0);

	/** Magnitudes for Pitch when Ready
	 * X - min
	 * Y - max
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	FVector2D ReadyPitchMagnitudes = FVector2D::ZeroVector;

	/** Magnitudes for Yaw when Ready
	 * X - min min
	 * Y - min max
	 * Z - max min
	 * W - max max
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	FVector4 ReadyYawMagnitudes = FVector4(0, 0, 0, 0);

	/** Magnitudes for Roll when Ready
	 * X - min min
	 * Y - min max
	 * Z - max min
	 * W - max max
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	FVector4 ReadyRollMagnitudes = FVector4(0, 0, 0, 0);

	/** Magnitudes for Kickback when aiming
	 * X - min
	 * Y - max
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	FVector2D AimingKickbackMagnitudeRange = FVector2D::ZeroVector;

	/** Magnitudes for Kickback Right when aiming
	 * X - min
	 * Y - max
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	FVector2D AimingKickbackRightwardMagnitudeRange = FVector2D::ZeroVector;

	/** Magnitudes for Kickback Up when aiming
	 * X - min
	 * Y - max
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	FVector2D AimingKickbackUpwardMagnitudeRange = FVector2D::ZeroVector;

	/** Magnitudes for Kickback when Ready
	 * X - min
	 * Y - max
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	FVector2D ReadyKickbackMagnitudeRange = FVector2D::ZeroVector;

	/** Magnitudes for Kickback Right when Ready
	 * X - min
	 * Y - max
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	FVector2D ReadyKickbackRightwardMagnitudeRange = FVector2D::ZeroVector;

	/** Magnitudes for Kickback Up when Ready
	 * X - min
	 * Y - max
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	FVector2D ReadyKickbackUpwardMagnitudeRange = FVector2D::ZeroVector;

	/** Rotation Smoothing amount in Auto Fire Mode */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Smoothing")
	FRotator AutoFireModeRotationSmoothingMagnitudes = FRotator::ZeroRotator;

	/** Location Smoothing amount in Auto Fire Mode */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Smoothing")
	FVector AutoFireModeLocationSmoothingMagnitudes = FVector::ZeroVector;

	/** Magnitude for each rotation axis while in Semi/Auto Fire Mode */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Smoothing")
	FRotator RotationAxesMagnitudes = FRotator(1.f, 1.f, 1.f);

	/** Magnitude for each translation axis while in Semi/Auto Fire Mode */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Smoothing")
	FVector LocationAxesMagnitudes = FVector(1.f, 1.f, 1.f);

	/** Enables smoothing for Roll Magnitudes */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Smoothing")
	bool bEnableRollSmoothing = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Noise 2D Layer")
	FVector2D Noise2DTarget = FVector2D::ZeroVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Noise 2D Layer")
	FVector2D Noise2DAccel = FVector2D::ZeroVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Noise 2D Layer")
	FVector2D Noise2DZero = FVector2D::ZeroVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Noise 2D Layer")
	float AimingNoiseMagnitude = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Pushback")
	float Pushback = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Pushback")
	float PushbackZero = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Pushback")
	float PushbackAccel = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Progression")
	FAlsxtProceduralRecoilAnimationInterpolationSettings PitchProgress;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Progression")
	FAlsxtProceduralRecoilAnimationInterpolationSettings ZProgress;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Progression")
	float ProgressAimAlpha = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Recoil Sway")
	FAlsxtProceduralRecoilSwaySettings RecoilSway;

	/** Playback position used for getting start value during Semi-Auto transition */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timeline")
	float PlaybackOffset = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timeline")
	float PlayRate = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Pivot")
	FVector PivotOffset = FVector::ZeroVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Pivot")
	FVector AdsPivotOffset = FVector::ZeroVector;
};