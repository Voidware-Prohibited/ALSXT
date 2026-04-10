#pragma once
#include "Utility/AlsGameplayTags.h"
#include "Utility/AlsxtGameplayTags.h"
#include "Utility/AlsxtMaterialStructs.h"
#include "Core/CameraVariableCollection.h"
#include "AlsxtViewSettings.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtFloatRangeClamp
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (EditCondition = "bClampMin"))
	float MinValue{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (EditCondition = "bClampMax"))
	float MaxValue{ 0.0f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Clamp")
	bool bClampMin {false};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Clamp")
	bool bClampMax{false};
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtFloatRange2dClamp
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Clamp")
	FAlsxtFloatRangeClamp ClampX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Clamp")
	FAlsxtFloatRangeClamp ClampY;
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtViewSettings
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Control Rotation")
	FAlsxtFloatRange2dClamp ControlRotationClamp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Focus")
	bool bEnableFocus {true};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Focus", meta = (EditCondition = "bEnableFocus"))
	float FocusZoomFactor{2.0f};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Freelook")
	bool bEnableFreelook {true};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Freelook", meta = (EditCondition = "bEnableFreelook"))
	FAlsxtFloatRange2dClamp ThirdPersonFreelookRotationClamp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Freelook", meta = (EditCondition = "bEnableFreelook"))
	FAlsxtFloatRange2dClamp FirstPersonFreelookRotationClamp;
	
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtViewCameraGaitSettings
{
	GENERATED_BODY()

public:
	
	// Currently, the direction-dependent movement speed can cause some jitter in multiplayer, so enable it at your own risk.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Features|Movement")
	uint8 bAllowDirectionDependentBoomOffset: 1 {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FVector WalkForwardBoomOffset{ForceInit};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS",
		Meta = (ClampMin = 0, EditCondition = "bAllowDirectionDependentBoomOffset"))
	FVector WalkBackwardBoomOffset{ForceInit};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	TSoftObjectPtr<UCameraShakeBase> WalkCameraShake {nullptr};

	// Target Values for Parameters specified in 
	UPROPERTY(EditAnywhere, Category="Post-Process Material", Meta = (AllowPrivateAccess))
	TArray<FAlsxtMaterialParametersSet> WalkingForwardMaterialEffectParameters;

	UPROPERTY(EditAnywhere, Category="Post-Process Material", Meta = (AllowPrivateAccess, EditCondition = "bAllowDirectionDependentBoomOffset"))
	TArray<FAlsxtMaterialParametersSet> WalkingBackwardMaterialEffectParameters;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FVector AimingForwardBoomOffset{ForceInit};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS",
		Meta = (ClampMin = 0, EditCondition = "bAllowDirectionDependentBoomOffset"))
	FVector AimingBackwardBoomOffset{ForceInit};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	TSoftObjectPtr<UCameraShakeBase> AimingCameraShake {nullptr};

	UPROPERTY(EditAnywhere, Category="Post-Process Material", Meta = (AllowPrivateAccess))
	TArray<FAlsxtMaterialParametersSet> AimingForwardMaterialEffectParameters;

	UPROPERTY(EditAnywhere, Category="Post-Process Material", Meta = (AllowPrivateAccess, EditCondition = "bAllowDirectionDependentBoomOffset"))
	TArray<FAlsxtMaterialParametersSet> AimingBackwardMaterialEffectParameters;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FVector CombatForwardBoomOffset{ForceInit};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS",
		Meta = (ClampMin = 0, EditCondition = "bAllowDirectionDependentBoomOffset"))
	FVector CombatBackwardBoomOffset{ForceInit};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	TSoftObjectPtr<UCameraShakeBase> CombatCameraShake {nullptr};

	UPROPERTY(EditAnywhere, Category="Post-Process Material", Meta = (AllowPrivateAccess))
	TArray<FAlsxtMaterialParametersSet> CombatForwardMaterialEffectParameters;

	UPROPERTY(EditAnywhere, Category="Post-Process Material", Meta = (AllowPrivateAccess, EditCondition = "bAllowDirectionDependentBoomOffset"))
	TArray<FAlsxtMaterialParametersSet> CombatBackwardMaterialEffectParameters;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FVector RunForwardBoomOffset{ForceInit};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS",
		Meta = (ClampMin = 0, EditCondition = "bAllowDirectionDependentBoomOffset"))
	FVector RunBackwardBoomOffset{ForceInit};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	TSoftObjectPtr<UCameraShakeBase> RunCameraShake {nullptr};

	UPROPERTY(EditAnywhere, Category="Post-Process Material", Meta = (AllowPrivateAccess))
	TArray<FAlsxtMaterialParametersSet> RunForwardMaterialEffectParameters;

	UPROPERTY(EditAnywhere, Category="Post-Process Material", Meta = (AllowPrivateAccess, EditCondition = "bAllowDirectionDependentBoomOffset"))
	TArray<FAlsxtMaterialParametersSet> RunBackwardMaterialEffectParameters;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FVector SprintBoomOffset{ForceInit};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	TSoftObjectPtr<UCameraShakeBase> SprintCameraShake {nullptr};

	UPROPERTY(EditAnywhere, Category="Post-Process Material", Meta = (AllowPrivateAccess))
	TArray<FAlsxtMaterialParametersSet> SprintForwardMaterialEffectParameters;

	UPROPERTY(EditAnywhere, Category="Post-Process Material", Meta = (AllowPrivateAccess, EditCondition = "bAllowDirectionDependentBoomOffset"))
	TArray<FAlsxtMaterialParametersSet> SprintBackwardMaterialEffectParameters;
public:
	FVector GetWalkBoomOffset() const;
	FVector GetAimingBoomOffset() const;
	FVector GetCombatBoomOffset() const;
	FVector GetRunBoomOffset() const;
	FVector GetSprintBoomOffset() const;
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtViewCameraStanceSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ForceInlineRow))
	TMap<FGameplayTag, FAlsxtViewCameraGaitSettings> Stances
	{
			{AlsStanceTags::Standing, {}},
			{AlsStanceTags::Crouching, {}},
			{AlsStanceTags::Prone, {}}
		
	};
};

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UAlsxtViewCameraSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Third Person", Meta = (ForceInlineRow))
	TMap<FGameplayTag, FAlsxtViewCameraStanceSettings> RotationModes
	{
		{AlsRotationModeTags::VelocityDirection, {}},
		{AlsRotationModeTags::ViewDirection, {}},
		{AlsRotationModeTags::Aiming, {}}
	};

	// Automatically interpolate Parameters for Post Process Materials by including them in this Parameter collection and define the Target values in Camera settings above
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera|Effects")
	TSoftObjectPtr<UMaterialParameterCollection> CameraEffectsMaterialParameterCollection {nullptr};
};

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UAlsxtGeneralViewSettingsDataAsset: public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "View Mode", meta = (AllowPrivateAccess))
	FGameplayTag DefaultViewMode {AlsViewModeTags::FirstPerson};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Features")
	bool bEnableSwitchCameraShoulder {true};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Features")
	bool bEnableFreelook {true};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Features")
	bool bEnableFocus {true};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Control Rotation")
	bool bClampControlRotation {false};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Control Rotation", meta = (EditCondition = "bClampControlRotation"))
	FAlsxtFloatRange2dClamp ThirdPersonControlRotationClamp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Freelook", meta = (EditCondition = "bEnableFreelook"))
	FAlsxtFloatRange2dClamp ThirdPersonFreelookRotationClamp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Freelook", meta = (EditCondition = "bEnableFreelook"))
	FAlsxtFloatRange2dClamp FirstPersonFreelookRotationClamp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera Shoulder", meta = (EditCondition = "bEnableSwitchCameraShoulder"))
	float CameraShoulderLeftOffset{-35.0f};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera Shoulder", meta = (EditCondition = "bEnableSwitchCameraShoulder"))
	float CameraShoulderRightOffset{35.0f};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Focus", meta = (EditCondition = "bEnableFocus"))
	float FocusZoomFactor{2.0f};

	// Control various Camera Parameters from nearly anywhere, similar to Material Parameter Collections
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	TSoftObjectPtr<UCameraVariableCollection> CameraVariablesCollection {nullptr};

	// Automatically interpolate Parameters for Post Process Materials by including them in this Parameter collection and define the Target values in Camera settings above
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	TSoftObjectPtr<UMaterialParameterCollection> CameraEffectsMaterialParameterCollection {nullptr};

	// General Settings for Gameplay Camera System
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ForceInlineRow))
	TSoftObjectPtr<UAlsxtViewCameraSettings> GeneralViewCameraSettings {nullptr};
};

inline FVector FAlsxtViewCameraGaitSettings::GetWalkBoomOffset() const
{
	return bAllowDirectionDependentBoomOffset
			   ? FVector::Min(WalkForwardBoomOffset, WalkBackwardBoomOffset)
			   : WalkForwardBoomOffset;
}

inline FVector FAlsxtViewCameraGaitSettings::GetAimingBoomOffset() const
{
	return bAllowDirectionDependentBoomOffset
			   ? FVector::Min(AimingForwardBoomOffset, AimingBackwardBoomOffset)
			   : AimingForwardBoomOffset;
}

inline FVector FAlsxtViewCameraGaitSettings::GetCombatBoomOffset() const
{
	return bAllowDirectionDependentBoomOffset
			   ? FVector::Min(CombatForwardBoomOffset, CombatBackwardBoomOffset)
			   : CombatForwardBoomOffset;
}

inline FVector FAlsxtViewCameraGaitSettings::GetRunBoomOffset() const
{
	return bAllowDirectionDependentBoomOffset
			   ? FVector::Min(RunForwardBoomOffset, RunBackwardBoomOffset)
			   : RunForwardBoomOffset;
}

inline FVector FAlsxtViewCameraGaitSettings::GetSprintBoomOffset() const
{
	return SprintBoomOffset;
}
