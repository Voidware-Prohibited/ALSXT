#pragma once

#include "Engine/EngineTypes.h"
#include "ALSXTCameraEffectsSettings.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTGeneralCameraEffectsSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Features")
	bool bEnableEffects { true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Features")
	bool bEnableDepthOfFieldEffect { true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Features")
	bool bEnableRadialBlurEffect { true };

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
	TEnumAsByte<ETraceTypeQuery> TraceType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Depth Of Field|First Person")
	float FirstPersonMinFStop { 0.025 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Depth Of Field|First Person")
	float FirstPersonMaxFStop { 0.2 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Depth Of Field|First Person")
	float FirstPersonAimedMinFStop { 0.025 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Depth Of Field|First Person")
	float FirstPersonAimedMaxFStop { 0.2 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Depth Of Field|Third Person")
	float ThirdPersonMinFStop { 0.2 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Depth Of Field|Third Person")
	float ThirdPersonMaxFStop { 0.5 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Depth Of Field|Third Person")
	float ThirdPersonAimedMinFStop { 0.025 };

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
