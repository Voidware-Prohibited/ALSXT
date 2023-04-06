#pragma once

#include "ALSXTCameraEffectsSettings.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTGeneralCameraEffectsSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnableCameraEffects { true };

};
