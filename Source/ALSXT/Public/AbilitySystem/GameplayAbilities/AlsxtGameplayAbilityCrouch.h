// MIT

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GameplayAbilities/AlsxtGameplayAbilityBase.h"
#include "AlsxtGameplayAbilityCrouch.generated.h"

struct FEventData;
/**
 * 
 */
UCLASS()
class ALSXT_API UAlsxtGameplayAbilityCrouch : public UAlsxtGameplayAbilityBase
{
	GENERATED_BODY()
	
public:
	UAlsxtGameplayAbilityCrouch();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr,
		FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Sprint")
	TSubclassOf<UGameplayEffect> StaminaRegenEffect;

	// Tag to set the magnitude of the stamina cost effect
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sprint|Cost")
	FGameplayTag StaminaCostTag;

	UPROPERTY()
	FGameplayEffectSpecHandle StaminaDrainEffectSpecHandle;
};
	