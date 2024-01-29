#pragma once

#include "Engine/EngineTypes.h"
#include "Engine/DataAsset.h"
#include "Camera/CameraShakeBase.h"
#include "ALSXTCameraEffectsSettings.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTMovementCameraShakeSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Shake|Third Person")
	TSubclassOf<UCameraShakeBase> ThirdPersonDefaultCameraShake{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Shake|Third Person")
	TSubclassOf<UCameraShakeBase> ThirdPersonWalkCameraShake{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Shake|Third Person")
	TSubclassOf <UCameraShakeBase> ThirdPersonRunCameraShake{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Shake|Third Person")
	TSubclassOf <UCameraShakeBase> ThirdPersonSprintCameraShake{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Shake|Third Person")
	TSubclassOf<UCameraShakeBase> ThirdPersonCrouchCameraShake{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Shake|Third Person")
	TSubclassOf <UCameraShakeBase> ThirdPersonCrouchWalkCameraShake{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Shake|Third Person")
	TSubclassOf <UCameraShakeBase> ThirdPersonCrouchRunCameraShake{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Shake|First Person")
	TSubclassOf <UCameraShakeBase> FirstPersonDefaultCameraShake{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Shake|First Person")
	TSubclassOf <UCameraShakeBase> FirstPersonWalkCameraShake{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Shake|First Person")
	TSubclassOf <UCameraShakeBase> FirstPersonRunCameraShake{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Shake|First Person")
	TSubclassOf <UCameraShakeBase> FirstPersonSprintCameraShake{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Shake|First Person")
	TSubclassOf <UCameraShakeBase> FirstPersonCrouchCameraShake{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Shake|First Person")
	TSubclassOf <UCameraShakeBase> FirstPersonCrouchWalkCameraShake{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Shake|First Person")
	TSubclassOf <UCameraShakeBase> FirstPersonCrouchRunCameraShake{ nullptr };
};

UCLASS()
class ALSXT_API UALSXTCameraShakeSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera Shake")
	FALSXTMovementCameraShakeSettings CameraShakeSettings;
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTGeneralCameraEffectsSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Features")
	bool bEnableEffects { true };

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
