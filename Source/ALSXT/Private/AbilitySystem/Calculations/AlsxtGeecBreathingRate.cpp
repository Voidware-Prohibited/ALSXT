// MIT

#include "AbilitySystem/Calculations/AlsxtGeecBreathingRate.h"
#include "AbilitySystem/AttributeSets/AlsxtStaminaAttributeSet.h"
#include "AbilitySystem/AttributeSets/AlsxtBreathAttributeSet.h"
#include "GameplayEffect.h"
#include "AbilitySystemComponent.h"
#include "AlsCharacter.h"
#include "AlsxtCharacter.h"
#include "Chaos/PBDSuspensionConstraintData.h"

// Define the attribute capture macro. This helps ensure proper attribute access.
// DEFINE_ATTRIBUTE_CAPTUREDEF(UAlsxtStaminaAttributeSet, CurrentStamina, Target, false);
// #define DEFINE_ATTRIBUTE_CAPTUREDEF(Attribute, CaptureType) \
// 	Attribute##Def(UAlsxtStaminaAttributeSet::Get##Attribute##Attribute(), CaptureType)

struct FAlsxtBreathingRateStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(CurrentStamina);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MaxStamina);

	FAlsxtBreathingRateStatics()
	{
		// Capture the Target's Stamina attribute.
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAlsxtStaminaAttributeSet, CurrentStamina, Target, false);
		// Capture the Target's MaxStamina attribute.
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAlsxtStaminaAttributeSet, MaxStamina, Target, false);

	}
};

static const FAlsxtBreathingRateStatics& AlsxtBreathingRateStatics()
{
	static FAlsxtBreathingRateStatics Statics;
	return Statics;
}

UAlsxtGeecBreathingRate::UAlsxtGeecBreathingRate()
{
	// Define the attributes to capture from the target.
	// `Target` indicates the character receiving the Gameplay Effect.
	// `false` means we don't need a snapshot of the attribute at the start.
	StaminaDef = FGameplayEffectAttributeCaptureDefinition(UAlsxtStaminaAttributeSet::GetCurrentStaminaAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);
	MaxStaminaDef = FGameplayEffectAttributeCaptureDefinition(UAlsxtStaminaAttributeSet::GetMaxStaminaAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);
	StaminaDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	MaxStaminaDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	// UAlsxtGeecBreathingRate::FBreathingRateCaptureDefinition UAlsxtGeecBreathingRate::BreathingRateCaptureDefinition;

	// Add the capture definitions to the relevant attributes.
	// Add the captured attributes to the RelevantAttributesToCapture array.
	RelevantAttributesToCapture.Add(AlsxtBreathingRateStatics().CurrentStaminaDef);
	RelevantAttributesToCapture.Add(AlsxtBreathingRateStatics().MaxStaminaDef);
	// RelevantAttributesToCapture.Add(StaminaDef);
	// RelevantAttributesToCapture.Add(MaxStaminaDef);
	// RelevantAttributesToCapture.Add(BreathingRateCaptureDefinition.HoldingBreathLengthDef);
}

void UAlsxtGeecBreathingRate::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	// --- Get information from the execution parameters ---
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FAggregatorEvaluateParameters EvaluationParameters;

	// Get the ability system component of the character receiving the effect.
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	if (!TargetAbilitySystemComponent)
	{
		return;
	}

	// --- Capture attribute values ---
	float Stamina = 1.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(StaminaDef, EvaluationParameters, Stamina);

	float MaxStamina = 1.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(MaxStaminaDef, EvaluationParameters, MaxStamina);

	// --- Perform the calculation logic ---
	float BreathingRate = 0.5f;
	float BreathingMagnitude = 0.5f;
	
	if (MaxStamina > 0.0f)
	{
		float StaminaRatio = Stamina / MaxStamina;

		// Inverse relationship: lower stamina means higher breathing rate.
		BreathingRate = FMath::Lerp(2.0f, 0.2f, FMath::Clamp(StaminaRatio, 0.0f, 1.0f));
		BreathingMagnitude = FMath::Lerp(2.0f, 0.2f, FMath::Clamp(StaminaRatio, 0.0f, 1.0f)); 
		
		// For example, if Stamina is 100% (Ratio 1.0), BreathingRate = 1.0 (calm).
		// If Stamina is 0% (Ratio 0.0), BreathingRate = 5.0 (heavy breathing).
	}
	else
	{
		// Default to a resting rate if max stamina is zero or invalid.
		BreathingRate = 0.2f;
		BreathingMagnitude = 0.2f;
	}

	

	float BreathingRateValue = 0.5f;
	float BreathingMagnitudeValue = 0.5f;
	FGameplayEffectAttributeCaptureDefinition BreathRateGameplayEffectAttributeCaptureDefinition = FGameplayEffectAttributeCaptureDefinition();
	BreathRateGameplayEffectAttributeCaptureDefinition.AttributeToCapture = UAlsxtBreathAttributeSet::GetCurrentBreathRateAttribute();
	BreathRateGameplayEffectAttributeCaptureDefinition.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(BreathRateGameplayEffectAttributeCaptureDefinition, FAggregatorEvaluateParameters(), BreathingRateValue);

	FGameplayEffectAttributeCaptureDefinition BreathAlphaGameplayEffectAttributeCaptureDefinition = FGameplayEffectAttributeCaptureDefinition();
	BreathAlphaGameplayEffectAttributeCaptureDefinition.AttributeToCapture = UAlsxtBreathAttributeSet::GetCurrentBreathRateAttribute();
	BreathAlphaGameplayEffectAttributeCaptureDefinition.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(BreathAlphaGameplayEffectAttributeCaptureDefinition, FAggregatorEvaluateParameters(), BreathingMagnitudeValue);
	
	// --- Apply the output modifiers ---
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UAlsxtBreathAttributeSet::GetCurrentBreathRateAttribute(), EGameplayModOp::Override, BreathingRateValue));
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UAlsxtBreathAttributeSet::GetCurrentBreathMagnitudeAttribute(), EGameplayModOp::Override, BreathingMagnitudeValue));

	AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetOwnerActor() : nullptr;
	AAlsxtCharacter* AlsxtCharacter = Cast<AAlsxtCharacter>(TargetActor);

	if (!AlsxtCharacter)
	{
		return;
	}

	// Update the struct in the Character
	AlsxtCharacter->AnimationParametersState.BreathingRate = BreathingRateValue;
	AlsxtCharacter->AnimationParametersState.BreathingAlpha = BreathingMagnitudeValue;
}
