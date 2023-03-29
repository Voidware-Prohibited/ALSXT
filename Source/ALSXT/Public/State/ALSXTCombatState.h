#pragma once

#include "Settings/ALSXTCombatSettings.h"
#include "ALSXTCombatState.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTCombatState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FALSXTCombatParameters CombatParameters;
};

