#pragma once

#include "Engine/EngineTypes.h"
#include "ALSXTDefensiveModeSettings.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTDefensiveModeSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	TArray<TEnumAsByte<EObjectTypeQuery>> AnticipationTraceObjectTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	TArray<TEnumAsByte<EObjectTypeQuery>> FallingAnticipationTraceObjectTypes;
};
