#pragma once

#include "ALSXTProceduralAimSettings.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTProceduralAimSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnableProceduralAim{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0))
	float AimedMaxTargetTraceDistance{ 500.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnableCombatProceduralAim{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0))
	float CombatMaxTargetTraceDistance{ 100.0 };
};
