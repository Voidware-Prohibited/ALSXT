#pragma once

#include "Utility/ALSXTStructs.h"
#include "ALSXTGripPositionSettings.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTGripPositionSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FForegripPositions ForegripPositions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Debug {false};
};
