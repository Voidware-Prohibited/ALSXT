// MIT

#pragma once

#include "CoreMinimal.h"
#include "AlsxtGameplayAbilityExternalCooldowns.h"
#include "AlsxtGameplayAbilitySprint.generated.h"

struct FEventData;
/**
 * 
 */
UCLASS()
class ALSXT_API UAlsxtGameplayAbilitySprint : public UAlsxtGameplayAbilityExternalCooldowns
{
	GENERATED_BODY()
	
public:
	UAlsxtGameplayAbilitySprint();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr,
		FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

protected:
	// Tag to set the magnitude of the stamina cost effect
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sprint|Cost")
	FGameplayTag StaminaCostTag;

	UPROPERTY()
	FGameplayEffectSpecHandle StaminaDrainEffectSpecHandle;

	UPROPERTY()
	FGameplayTag PreviousGait;
};
	