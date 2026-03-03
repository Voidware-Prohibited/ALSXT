// MIT

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "AlsxtGeecStaminaDrainRate.generated.h"

/**
 * 
 */
UCLASS()
class ALSXT_API UAlsxtGeecStaminaDrainRate : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UAlsxtGeecStaminaDrainRate();
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
