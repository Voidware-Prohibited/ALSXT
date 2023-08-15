#pragma once

#include "Animation/AnimMontage.h"
#include "ALSXTFootstepEffectSettings.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTGeneralFootstepEffectSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnableFootstepEffects { true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnableEquipmentSounds{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnableOverlayObjectSounds{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnableVertexPaintTrace { true };

};
