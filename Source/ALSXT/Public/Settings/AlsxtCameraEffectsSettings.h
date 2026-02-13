#pragma once

#include "Engine/EngineTypes.h"
#include "Engine/DataAsset.h"
#include "Camera/CameraShakeBase.h"
#include "Curves/CurveVector.h"
#include "Settings/AlsxtFPEyeFocusSettings.h"
#include "GameplayTagContainer.h"
#include "Utility/AlsGameplayTags.h"
#include "AlsxtCameraEffectsSettings.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtCameraShakeSetting
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UCameraShakeBase> CameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseMagnitude{ 1.0f };

	bool operator==(const FAlsxtCameraShakeSetting& other) const
	{
		return (other.CameraShake == CameraShake) && (other.BaseMagnitude == BaseMagnitude);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtStanceMovementCameraViewShakeSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Als.Gait"))
	TMap<UPARAM(meta = (Categories = "Als.Gait")) FGameplayTag, FAlsxtCameraShakeSetting> Gaits;

	bool operator==(const FAlsxtStanceMovementCameraViewShakeSettings& other) const
	{
		return (other.Gaits.OrderIndependentCompareEqual(Gaits));
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtMovementCameraViewShakeSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Als.Stance"))
	TMap<UPARAM(meta = (Categories = "Als.Stance")) FGameplayTag, FAlsxtStanceMovementCameraViewShakeSettings> Grounded;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Als.Gait"))
	TMap<UPARAM(meta = (Categories = "Als.Gait")) FGameplayTag, FAlsxtCameraShakeSetting> Ladder;
	
	bool operator==(const FAlsxtMovementCameraViewShakeSettings& other) const
	{
		return (other.Grounded.OrderIndependentCompareEqual(Grounded)) && (other.Ladder.OrderIndependentCompareEqual(Ladder));
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtMovementCameraShakeSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseCameraShakeForOverlayModes{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAlsxtMovementCameraViewShakeSettings ThirdPerson;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAlsxtMovementCameraViewShakeSettings FirstPerson;

	bool operator==(const FAlsxtMovementCameraShakeSettings& other) const
	{
		return (other.bUseCameraShakeForOverlayModes == bUseCameraShakeForOverlayModes) && (other.ThirdPerson == ThirdPerson) && (other.FirstPerson == FirstPerson);
	}
};

UCLASS()
class ALSXT_API UAlsxtCameraShakeSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera Shake")
	FAlsxtMovementCameraShakeSettings CameraShakeSettings;
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtMovementRotationModeShakeSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ForceInlineRow))
	TMap<FGameplayTag, FAlsxtMovementCameraShakeSettings> Grounded
	{
		{AlsRotationModeTags::VelocityDirection, {}},
		{AlsRotationModeTags::ViewDirection, {}},
		{AlsRotationModeTags::Aiming, {}}
	};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Als.Gait"))
	TMap<UPARAM(meta = (Categories = "Als.Gait")) FGameplayTag, FAlsxtCameraShakeSetting> Ladder;
	
};

UCLASS()
class ALSXT_API UAlsxtMovementCameraShakeSettingsAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera Shake")
	FAlsxtMovementRotationModeShakeSettings MovementCameraShakeSettings;
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtGeneralCameraEffectsSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Features")
	bool bEnableEffects { true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Features|Eye Focus")
	bool bEnableEyeFocus{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Features|Eye Focus")
	bool bEnableFirstPersonEyeFocusVignette{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Features|Camera Shake")
	bool bEnableMovementCameraShake{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Features|Camera Shake")
	bool bEnableThirdPersonDefaultCameraShake{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Features|Camera Shake")
	bool bEnableFirstPersonDefaultCameraShake{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Features|Camera Shake")
	bool bEnableWalkingCameraShake{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Features|Camera Shake")
	bool bEnableRunningCameraShake{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Features|Camera Shake")
	bool bEnableSprintingCameraShake{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Features")
	bool bEnableDepthOfFieldEffect { true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Features")
	bool bEnableRadialBlurEffect { true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Features")
	bool bEnableFrostEffect{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Features")
	bool bEnableSuppressionEffect { true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Features")
	bool bEnableBlindnessEffect { true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Features")
	bool bEnableConcussionEffect { true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Features")
	bool bEnableDamageEffect { true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Features")
	bool bEnableDeathEffect { true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Features")
	bool bEnableDrunkEffect { true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Features")
	bool bEnableHighEffect { true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Features|Debug")
	bool bDebugMode { false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eye Focus")
	FAlsxtFPEyeFocusSettings FirstPersonEyeFocusSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Depth Of Field")
	float DepthOfFieldEffectAmount { 1.0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Depth Of Field|Combat")
	bool bFocusOnCombatTarget { true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Depth Of Field")
	float MaxDOFTraceDistance { 12000.0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Depth Of Field")
	UCurveVector* FirstPersonFocalDistanceToFStopCurve {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Depth Of Field")
	UCurveVector* ThirdPersonFocalDistanceToFStopCurve {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Depth Of Field")
	TEnumAsByte<ETraceTypeQuery> TraceType {TraceTypeQuery1};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Depth Of Field|Third Person")
	float ThirdPersonAimedMaxFStop { 0.175 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Depth Of Field|Third Person")
	float DepthOfFieldDepthBlurAmount {1.0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Depth Of Field|Third Person")
	float DepthOfFieldDepthBlurRadius {1.0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Depth Of Field|Third Person")
	float DepthOfFieldNearBlurSize {1.0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Depth Of Field|Third Person")
	float DepthOfFieldFarBlurSize {0.2f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Depth Of Field|Third Person")
	float DepthOfFieldSkyFocusDistance {12000.0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Depth Of Field|Third Person")
	float DepthOfFieldSqueezeFactor {1.0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Depth Of Field|Third Person")
	float DepthOfFieldVignetteSize {1.0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radial Blur Effect")
	TObjectPtr<UMaterialInstance> RadialBlurMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radial Blur Effect")
	float RadialBlurEffectAmount { 1.0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radial Blur Effect")
	float RadialBlurMaxVelocity {2500.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radial Blur Effect")
	float RadialBlurMaxWeight {1.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drunk Effect")
	TObjectPtr<UMaterialInstance> DrunkEffectMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drunk Effect")
	float DrunkEffectAmount { 1.0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drunk Effect")
	float DrunkEffectMaxWeight { 1.0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drunk Effect")
	float DrunkEffectBaseFadeOutSpeed { 1.0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "High Effect")
	TObjectPtr<UMaterialInstance> HighEffectMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "High Effect")
	float HighEffectAmount { 1.0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "High Effect")
	float HighEffectMaxWeight { 1.0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "High Effect")
	float HighEffectBaseFadeOutSpeed { 1.0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Suppression Effect")
	TObjectPtr<UMaterialInstance> SuppressionEffectMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Suppression Effect")
	float SuppressionEffectAmount { 1.0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Suppression Effect")
	float SuppressionEffectMaxWeight{ 1.0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Suppression Effect")
	float SuppressionEffectBaseFadeOutSpeed { 1.0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blindness Effect")
	TObjectPtr<UMaterialInstance> BlindnessEffectMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blindness Effect")
	float BlindnessEffectAmount { 1.0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blindness Effect")
	float BlindnessEffectMaxWeight { 1.0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blindness Effect")
	float BlindnessEffectBaseFadeOutSpeed { 1.0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Concussion Effect")
	TObjectPtr<UMaterialInstance> ConcussionEffectMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Concussion Effect")
	float ConcussionEffectAmount { 1.0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Concussion Effect")
	float ConcussionEffectMaxWeight { 1.0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Concussion Effect")
	float ConcussionEffectBaseFadeOutSpeed { 1.0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Effect")
	TObjectPtr<UMaterialInstance> DamageEffectMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Effect")
	float DamageEffectAmount { 1.0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Effect")
	float DamageEffectMaxWeight { 1.0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Effect")
	float DamageEffectBaseFadeOutSpeed { 1.0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death Effect")
	TObjectPtr<UMaterialInstance> DeathEffectMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death Effect")
	float DeathEffectAmount { 1.0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death Effect")
	float DeathEffectMaxWeight { 1.0 };
};
