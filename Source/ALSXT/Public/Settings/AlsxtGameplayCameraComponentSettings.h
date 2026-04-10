#pragma once

#include "Engine/EngineTypes.h"
#include "Engine/DataAsset.h"
#include "Camera/CameraShakeBase.h"
#include "GameplayTagContainer.h"
#include "Utility/AlsxtMaterialStructs.h"
#include "AlsxtGameplayCameraComponentSettings.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtCameraShakeMultipliers
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float AmplitudeMultiplier {1.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float FrequencyMultiplier {1.0f};
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtCameraShakeEntry
{
	GENERATED_BODY()

	// Movement Camera Shake for Specific State. If an Asset is selected it will override the Parent Camera Shake properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Shake", meta=(DisplayThumbnail = "false"))
	TSubclassOf<UCameraShakeBase> MovementCameraShake {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess, ForceInlineRow))
	FAlsxtCameraShakeMultipliers Multipliers;
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtStateCameraOffsetSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Offset")
	FAlsxtVectorInterpolationParams RelaxedOffset;

	// Movement Camera Shake for Specific State. If an Asset is selected it will override the Parent Camera Shake properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Shake", meta=(DisplayThumbnail = "false"))
	FAlsxtCameraShakeEntry RelaxedCameraShake;

	// Matching Material Parameters in the CameraEffectsMaterialCollection will be Interpolated for this state according to the Interpolation Rule
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Post-Process Material")
	FAlsxtMaterialParametersSet RelaxedMaterialEffectParameters;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Offset")
	FAlsxtVectorInterpolationParams ReadyOffset;

	// Movement Camera Shake for Specific State. If an Asset is selected it will override the Parent Camera Shake properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Shake", meta=(DisplayThumbnail = "false"))
	FAlsxtCameraShakeEntry ReadyCameraShake;

	// Matching Material Parameters in the CameraEffectsMaterialCollection will be Interpolated for this state according to the Interpolation Rule
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Post-Process Material")
	FAlsxtMaterialParametersSet ReadyMaterialEffectParameters;

	// Overlay Object Settings will override this property.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Offset")
	FAlsxtVectorInterpolationParams AimingOffset;

	// Overlay Object Settings will override this property.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Shake", meta=(DisplayThumbnail = "false"))
	FAlsxtCameraShakeEntry AimingCameraShake;

	// Overlay Object Settings will override this property.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Post-Process Material")
	FAlsxtMaterialParametersSet AimingMaterialEffectParameters;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Offset")
	FAlsxtVectorInterpolationParams BlockingOffset;

	// Movement Camera Shake for Specific State. If an Asset is selected it will override the Parent Camera Shake properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Shake", meta=(DisplayThumbnail = "false"))
	FAlsxtCameraShakeEntry BlockingCameraShake;

	// Matching Material Parameters in the CameraEffectsMaterialCollection will be Interpolated for this state according to the Interpolation Rule
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Post-Process Material")
	FAlsxtMaterialParametersSet BlockingMaterialEffectParameters;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Offset")
	FAlsxtVectorInterpolationParams FocusOffset;

	// Movement Camera Shake for Specific State. If an Asset is selected it will override the Parent Camera Shake properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Shake", meta=(DisplayThumbnail = "false"))
	FAlsxtCameraShakeEntry FocusCameraShake;

	// Matching Material Parameters in the CameraEffectsMaterialCollection will be Interpolated for this state according to the Interpolation Rule
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Post-Process Material")
	FAlsxtMaterialParametersSet FocusMaterialEffectParameters;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Offset")
	FAlsxtVectorInterpolationParams VelocityDirectionOffset;

	bool operator==(const FAlsxtStateCameraOffsetSettings& other) const
	{
		return (other.RelaxedOffset == RelaxedOffset) && (other.ReadyOffset == ReadyOffset) && (other.FocusOffset == FocusOffset) && (other.AimingOffset == AimingOffset);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtCombatStanceCameraOffsetSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Alsxt.CombatStance"))
	TMap<UPARAM(meta = (Categories = "Alsxt.CombatStance")) FGameplayTag, FAlsxtStateCameraOffsetSettings> CombatStances	{
	{ FGameplayTag::RequestGameplayTag(TEXT("Als.Combat Stance.Orthodox")), FAlsxtStateCameraOffsetSettings() },
	{ FGameplayTag::RequestGameplayTag(TEXT("Als.Combat Stance.Southpaw")), FAlsxtStateCameraOffsetSettings() }
	};

	bool operator==(const FAlsxtCombatStanceCameraOffsetSettings& other) const
	{
		return (other.CombatStances.OrderIndependentCompareEqual(CombatStances));
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtDirectionCameraOffsetSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAlsxtCombatStanceCameraOffsetSettings Forward;

	// Only utilized if bUseMovementDependentCameraSettings is True, otherwise Forward is used for both directions.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAlsxtCombatStanceCameraOffsetSettings Backward;

	// Gait Movement Camera Shake. Can be overriden by CameraShake Child Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Shake", meta=(DisplayThumbnail = "false"))
	TSubclassOf<UCameraShakeBase> MovementCameraShake {nullptr};

	bool operator==(const FAlsxtDirectionCameraOffsetSettings& other) const
	{
		return (other.Forward == Forward) && (other.Backward == Backward) && (other.MovementCameraShake == MovementCameraShake);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtGaitCameraOffsetSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Als.Gait"))
	TMap<UPARAM(meta = (Categories = "Als.Gait")) FGameplayTag, FAlsxtDirectionCameraOffsetSettings> Gaits{
		{ FGameplayTag::RequestGameplayTag(TEXT("Als.Gait.Walking")), FAlsxtDirectionCameraOffsetSettings() },
		{ FGameplayTag::RequestGameplayTag(TEXT("Als.Gait.Combat")), FAlsxtDirectionCameraOffsetSettings() },
		{ FGameplayTag::RequestGameplayTag(TEXT("Als.Gait.Running")), FAlsxtDirectionCameraOffsetSettings() },
		{ FGameplayTag::RequestGameplayTag(TEXT("Als.Gait.Sprinting")), FAlsxtDirectionCameraOffsetSettings() }
	};

	bool operator==(const FAlsxtGaitCameraOffsetSettings& other) const
	{
		return (other.Gaits.OrderIndependentCompareEqual(Gaits));
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtStanceGameplayCameraOffsetSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Als.Stance"))
	TMap<UPARAM(meta = (Categories = "Als.Stance")) FGameplayTag, FAlsxtGaitCameraOffsetSettings> Stances{
		{ FGameplayTag::RequestGameplayTag(TEXT("Als.Stance.Standing")), FAlsxtGaitCameraOffsetSettings() },
		{ FGameplayTag::RequestGameplayTag(TEXT("Als.Stance.Crouching")), FAlsxtGaitCameraOffsetSettings() }
	};

	bool operator==(const FAlsxtStanceGameplayCameraOffsetSettings& other) const
	{
		return (other.Stances.OrderIndependentCompareEqual(Stances));
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtViewModeGameplayCameraOffsetSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Als.ViewMode"))
	TMap<UPARAM(meta = (Categories = "Als.ViewMode")) FGameplayTag, FAlsxtStanceGameplayCameraOffsetSettings> ViewModes{
		{ FGameplayTag::RequestGameplayTag(TEXT("Als.ViewMode.ThirdPerson")), FAlsxtStanceGameplayCameraOffsetSettings() },
		{ FGameplayTag::RequestGameplayTag(TEXT("Als.ViewMode.FirstPerson")), FAlsxtStanceGameplayCameraOffsetSettings() }
	};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseMovementDependentCameraSettings {false};

	bool operator==(const FAlsxtViewModeGameplayCameraOffsetSettings& other) const
	{
		return (other.ViewModes.OrderIndependentCompareEqual(ViewModes)) && (other.bUseMovementDependentCameraSettings == bUseMovementDependentCameraSettings);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtLocomotionModeGameplayCameraOffsetSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Als.LocomotionMode"))
	TMap<UPARAM(meta = (Categories = "Als.LocomotionMode")) FGameplayTag, FAlsxtViewModeGameplayCameraOffsetSettings> LocomotionModes{
			{ FGameplayTag::RequestGameplayTag(TEXT("Als.LocomotionMode.Grounded")), FAlsxtViewModeGameplayCameraOffsetSettings() },
			{ FGameplayTag::RequestGameplayTag(TEXT("Als.LocomotionMode.InAir")), FAlsxtViewModeGameplayCameraOffsetSettings() }
	};

	bool operator==(const FAlsxtLocomotionModeGameplayCameraOffsetSettings& other) const
	{
		return (other.LocomotionModes.OrderIndependentCompareEqual(LocomotionModes));
	}
};

UCLASS(Blueprintable)
class ALSXT_API UAlsxtOverlayCameraOffsetSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CameraOffsetSettings")
	FAlsxtLocomotionModeGameplayCameraOffsetSettings CameraOffsetSettings;
};
