// MIT

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AlsxtAttributeSetBase.h"
#include "AlsxtBreathAttributeSet.generated.h"


// Contains Attributes related to BreathRate. Any Character that uses BreathRate will need this Attribute Set.
//
//	Contents:
//	
//	CurrentBreathRate - Primarily used as a cost resource for Abilities.
//	MaximumBreathRate - The maximum amount of Breath the Character can have.
//	BreathRegeneration - How much Breath is gained per period of Regeneration.
UCLASS()
class ALSXT_API UAlsxtBreathAttributeSet : public UAlsxtAttributeSetBase
{
	GENERATED_BODY()

public:
	
	UAlsxtBreathAttributeSet();

	// Attribute Set Overrides.
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	virtual void ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const override;

	// Set Attributes to replicate.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Holds the current value for Breath.
	UPROPERTY(BlueprintReadOnly, Category = "Breath Attribute Set", ReplicatedUsing = OnRep_CurrentBreathRate)
	FGameplayAttributeData CurrentBreathRate;
	ATTRIBUTE_ACCESSORS_BASIC(UAlsxtBreathAttributeSet, CurrentBreathRate)

	// Holds the value for Maximum Breath.
	UPROPERTY(BlueprintReadOnly, Category = "Breath Attribute Set", ReplicatedUsing = OnRep_MaximumBreathRate)
	FGameplayAttributeData MaximumBreathRate;
	ATTRIBUTE_ACCESSORS_BASIC(UAlsxtBreathAttributeSet, MaximumBreathRate)

	// Holds the current value for Breath.
	UPROPERTY(BlueprintReadOnly, Category = "Breath Attribute Set", ReplicatedUsing = OnRep_CurrentBreathMagnitude)
	FGameplayAttributeData CurrentBreathMagnitude;
	ATTRIBUTE_ACCESSORS_BASIC(UAlsxtBreathAttributeSet, CurrentBreathMagnitude)

	// Holds the value for Maximum Breath.
	UPROPERTY(BlueprintReadOnly, Category = "Breath Attribute Set", ReplicatedUsing = OnRep_MaxBreathMagnitude)
	FGameplayAttributeData MaxBreathMagnitude;
	ATTRIBUTE_ACCESSORS_BASIC(UAlsxtBreathAttributeSet, MaxBreathMagnitude)

	// Holds the current value for Breath Regeneration.
	UPROPERTY(BlueprintReadOnly, Category = "Breath Attribute Set", ReplicatedUsing = OnRep_CurrentBreathRegeneration)
	FGameplayAttributeData CurrentBreathRegeneration;
	ATTRIBUTE_ACCESSORS_BASIC(UAlsxtBreathAttributeSet, CurrentBreathRegeneration)

	// Holds the current value for Breath Regeneration.
	UPROPERTY(BlueprintReadOnly, Category = "Breath Attribute Set", ReplicatedUsing = OnRep_MaxBreathRegeneration)
	FGameplayAttributeData MaxBreathRegeneration;
	ATTRIBUTE_ACCESSORS_BASIC(UAlsxtBreathAttributeSet, MaxBreathRegeneration)

protected:
	
	UFUNCTION()
	virtual void OnRep_CurrentBreathRate(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_MaximumBreathRate(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_CurrentBreathMagnitude(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_MaxBreathMagnitude(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_CurrentBreathRegeneration(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_MaxBreathRegeneration(const FGameplayAttributeData& OldValue);
};