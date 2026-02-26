// MIT

#pragma once

#include "CoreMinimal.h"
#include "AlsxtGameplayAbilityExternalCooldowns.h"
#include "AlsxtGameplayAbilityRoll.generated.h"

/**
 * 
 */
UCLASS()
class ALSXT_API UAlsxtGameplayAbilityRoll : public UAlsxtGameplayAbilityExternalCooldowns
{
	GENERATED_BODY()
	
public:
	UAlsxtGameplayAbilityRoll();

	// Default value for the stamina cost
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Roll|Cost")
	float BaseRollStaminaCost = 0.20f;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	// Tag to set the magnitude of the stamina cost effect
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Roll|Cost")
	FGameplayTag StaminaCostTag;

	UPROPERTY()
	FGameplayEffectSpecHandle StaminaDrainEffectSpecHandle;
	
};
