// MIT

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GameplayAbilities/AlsxtGameplayAbilityBase.h"
#include "AlsxtGameplayAbilityStaminaRegen.generated.h"

struct FEventData;
/**
 * 
 */
UCLASS()
class ALSXT_API UAlsxtGameplayAbilityStaminaRegen : public UAlsxtGameplayAbilityBase
{
	GENERATED_BODY()
	
public:
	UAlsxtGameplayAbilityStaminaRegen();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StaminaRegen")
	float BaseStaminaRegenPerSecond = 0.05f; // Base Cost per Second for sprint

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr,
		FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

protected:	
	UPROPERTY(EditDefaultsOnly, Category = "Stamina Regen")
	TSubclassOf<UGameplayEffect> StaminaRegenEffect;

	// Tag to set the magnitude of the stamina cost effect
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StaminaRegen")
	FGameplayTag StaminaRegenTag;

	UPROPERTY()
	FGameplayEffectSpecHandle StaminaRegenEffectSpecHandle;
};
	