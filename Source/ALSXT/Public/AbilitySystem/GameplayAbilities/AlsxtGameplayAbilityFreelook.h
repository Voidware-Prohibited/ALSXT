// MIT

#pragma once

#include "CoreMinimal.h"
#include "AlsxtGameplayAbilityBase.h"
#include "AlsxtGameplayAbilityFreelook.generated.h"

/**
 * 
 */
UCLASS()
class ALSXT_API UAlsxtGameplayAbilityFreelook : public UAlsxtGameplayAbilityBase
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
};
