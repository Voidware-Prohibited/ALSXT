#pragma once

#include "Settings/ALSXTClothingSettings.h"
#include "ALSXTClothingState.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTClothingState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FALSXTClothingParameters ClothingParameters;
};

