// MIT


#include "AbilitySystem/Calculations/AlsxtGeecStaminaRegenCooldown.h"

UAlsxtGeecStaminaRegenCooldown::UAlsxtGeecStaminaRegenCooldown()
{
}

void UAlsxtGeecStaminaRegenCooldown::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	// Define the tag used in the Gameplay Effect for "Set by Caller" magnitude
	FGameplayTag CooldownTag = FGameplayTag::RequestGameplayTag(FName("Data.CooldownDuration"));

	// Get the value set in the ability (e.g., using FGameplayEffectSpec::SetSetByCallerMagnitude)
	float CooldownDuration = 0.0f;
	Spec.GetSetByCallerMagnitude(CooldownTag.GetTagName(), true, CooldownDuration);

	// Apply the duration to the output.
	// NOTE: SetDuration is the correct, non-deprecated method as of recent UE versions to apply calculated duration.
	// OutExecutionOutput.SetDuration(CooldownDuration);
}
