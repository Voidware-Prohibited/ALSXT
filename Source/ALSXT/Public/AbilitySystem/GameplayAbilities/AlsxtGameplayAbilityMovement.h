// MIT

#pragma once

#include "CoreMinimal.h"
#include "AlsxtGameplayAbilityBase.h"
#include "AlsxtGameplayAbilityMovement.generated.h"

/**
 * 
 */
UCLASS()
class ALSXT_API UAlsxtGameplayAbilityMovement : public UAlsxtGameplayAbilityBase
{
	GENERATED_BODY()

public:
	UAlsxtGameplayAbilityMovement();

	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

protected:
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
};
