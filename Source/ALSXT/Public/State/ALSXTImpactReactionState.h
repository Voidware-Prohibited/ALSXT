#pragma once

#include "ALSXTImpactReactionState.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTImpactReactionState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	FALSXTImpactReactionParameters ImpactReactionParameters;
};

