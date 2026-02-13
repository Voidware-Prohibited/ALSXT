// MIT

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GameplayAbilities/AlsxtGameplayAbilityBase.h"
#include "AlsxtGameplayAbilityProne.generated.h"

struct FEventData;
/**
 * 
 */
UCLASS()
class ALSXT_API UAlsxtGameplayAbilityProne : public UAlsxtGameplayAbilityBase
{
	GENERATED_BODY()
	
public:
	UAlsxtGameplayAbilityProne();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stamina")
	float BaseStaminaCostPerSecond = 0.00f; // Base Cost per Second for running

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr,
		FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Sprint")
	TSubclassOf<UGameplayEffect> StaminaCostEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Sprint")
	TSubclassOf<UGameplayEffect> StaminaRegenEffect;

	// Tag to set the magnitude of the stamina cost effect
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sprint|Cost")
	FGameplayTag StaminaCostTag;

	UPROPERTY()
	FGameplayEffectSpecHandle StaminaDrainEffectSpecHandle;
};
	