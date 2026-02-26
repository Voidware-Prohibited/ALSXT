#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "AbilitySystem/AttributeSets/AlsxtStaminaAttributeSet.h"
#include "AbilitySystem/AttributeSets/AlsxtBreathAttributeSet.h"
#include "State/AlsxtAnimationParametersState.h"
#include "AlsxtGeecBreathingRate.generated.h"

/**
 * Custom execution calculation for breathing rate.
 */
UCLASS()
class ALSXT_API UAlsxtGeecBreathingRate : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	// Constructor where attribute captures are defined.
	UAlsxtGeecBreathingRate();

	// Default value for the stamina cost
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Jump|Cost")
	float MinBreathingRate = 0.50f;

	// Default value for the stamina cost
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Jump|Cost")
	float MaxBreathingRate = 2.0f;
	
	// Default value for the stamina cost
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Jump|Cost")
	float MinBreathingMagnitude = 0.50f;

	// Default value for the stamina cost
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Jump|Cost")
	float MaxBreathingMagnitude = 2.0f;

protected:
	// This is the core function where you perform the calculation.
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerUpdateAnimationParametersState(FAlsxtAnimationParametersState NewAnimationParametersState, const FGameplayEffectCustomExecutionParameters& ExecutionParams) const;

private:
	// Capture definition for the Stamina attribute.
	FGameplayEffectAttributeCaptureDefinition StaminaDef;

	// Capture definition for the MaxStamina attribute.
	FGameplayEffectAttributeCaptureDefinition MaxStaminaDef;
};