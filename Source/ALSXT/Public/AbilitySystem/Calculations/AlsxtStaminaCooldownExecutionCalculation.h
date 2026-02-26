// MIT

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "AlsxtStaminaCooldownExecutionCalculation.generated.h"

/**
 * 
 */
UCLASS()
class ALSXT_API UAlsxtStaminaCooldownExecutionCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UAlsxtStaminaCooldownExecutionCalculation();
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
