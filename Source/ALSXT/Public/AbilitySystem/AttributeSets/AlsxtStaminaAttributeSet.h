// Copyright (C) 2025 Uriel Ballinas, VOIDWARE Prohibited. All rights reserved.
// This software is licensed under the MIT License (LICENSE.md).

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AlsxtAttributeSetBase.h"
#include "AlsxtStaminaAttributeSet.generated.h"

/**
* @file AlsxtStaminaAttributeSet.h
* @brief Contains Attributes related to Stamina. Any Character that uses Stamina will need this Attribute Set.
* @attribute CurrentStamina - Primarily used as a cost resource for Abilities.
* @attribute MaxStamina - The Max amount of Stamina the Character can have.
* @attribute StaminaRegenMagnitude - How much Stamina is gained per period of Regen.
*/
UCLASS()
class ALSXT_API UAlsxtStaminaAttributeSet : public UAlsxtAttributeSetBase
{
	GENERATED_BODY()

public:
	UAlsxtStaminaAttributeSet();

	// Attribute Set Overrides.
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	virtual void ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const override;

	// Set Attributes to replicate.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	// Holds the current value for Stamina.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stamina Attribute Set", ReplicatedUsing = OnRep_CurrentStamina)
	FGameplayAttributeData CurrentStamina;
	ATTRIBUTE_ACCESSORS_BASIC(UAlsxtStaminaAttributeSet, CurrentStamina)

	// Holds the value for Max Stamina.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stamina Attribute Set", ReplicatedUsing = OnRep_MaxStamina)
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS_BASIC(UAlsxtStaminaAttributeSet, MaxStamina)

	// Holds the value for Max Stamina.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stamina Attribute Set", ReplicatedUsing = OnRep_LowStaminaThreshold)
	FGameplayAttributeData LowStaminaThreshold;
	ATTRIBUTE_ACCESSORS_BASIC(UAlsxtStaminaAttributeSet, LowStaminaThreshold)

	// Holds the current value for Stamina Regen.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stamina Attribute Set", ReplicatedUsing = OnRep_CurrentStaminaRegenMagnitude)
	FGameplayAttributeData CurrentStaminaRegenMagnitude;
	ATTRIBUTE_ACCESSORS_BASIC(UAlsxtStaminaAttributeSet, CurrentStaminaRegenMagnitude)

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stamina Attribute Set", ReplicatedUsing = OnRep_MaxStaminaRegenMagnitude)
	FGameplayAttributeData MaxStaminaRegenMagnitude;
	ATTRIBUTE_ACCESSORS_BASIC(UAlsxtStaminaAttributeSet, MaxStaminaRegenMagnitude);

	// Holds the current value for Stamina Regen.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stamina Attribute Set", ReplicatedUsing = OnRep_CurrentStaminaRegenDelay)
	FGameplayAttributeData CurrentStaminaRegenDelay;
	ATTRIBUTE_ACCESSORS_BASIC(UAlsxtStaminaAttributeSet, CurrentStaminaRegenDelay)

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stamina Attribute Set", ReplicatedUsing = OnRep_MaxStaminaRegenDelay)
	FGameplayAttributeData MaxStaminaRegenDelay;
	ATTRIBUTE_ACCESSORS_BASIC(UAlsxtStaminaAttributeSet, MaxStaminaRegenDelay);

protected:
	
	UFUNCTION()
	virtual void OnRep_CurrentStamina(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_MaxStamina(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_LowStaminaThreshold(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_CurrentStaminaRegenMagnitude(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_MaxStaminaRegenMagnitude(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_CurrentStaminaRegenDelay(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_MaxStaminaRegenDelay(const FGameplayAttributeData& OldValue);
};