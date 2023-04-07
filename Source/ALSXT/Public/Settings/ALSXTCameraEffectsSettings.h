#pragma once

#include "ALSXTCameraEffectsSettings.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTGeneralCameraEffectsSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnableEffects { true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnableDepthOfFieldEffect { true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DepthOfFieldEffectAmount { 1.0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnableRadialBlurEffect { true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RadialBlurEffecAmount { 1.0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnableSuppressionEffect { true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SuppressionEffecAmount { 1.0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnableDamageEffect { true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageEffecAmount { 1.0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnableDeathEffect { true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDebugMode { false };

};
