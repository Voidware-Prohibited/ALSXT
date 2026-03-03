// MIT

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "AlsxtGeecStaminaRegenCooldown.generated.h"

/**
 * 
 */
UCLASS()
class ALSXT_API UAlsxtGeecStaminaRegenCooldown : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UAlsxtGeecStaminaRegenCooldown();
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
