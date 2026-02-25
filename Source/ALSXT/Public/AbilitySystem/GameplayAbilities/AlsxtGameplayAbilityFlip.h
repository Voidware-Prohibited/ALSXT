// MIT

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GameplayAbilities/AlsxtGameplayAbilityBase.h"
#include "AlsxtGameplayAbilityFlip.generated.h"

/**
 * 
 */
UCLASS()
class ALSXT_API UAlsxtGameplayAbilityFlip : public UAlsxtGameplayAbilityBase
{
	GENERATED_BODY()
	
public:
	UAlsxtGameplayAbilityFlip();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr,
		FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

protected:
	// The Gameplay Effect for the stamina cost
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flip|Cost")
	TSubclassOf<UGameplayEffect> StaminaCostEffect;

	// Tag to set the magnitude of the stamina cost effect
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flip|Cost")
	FGameplayTag StaminaCostTag;

	UPROPERTY()
	FGameplayEffectSpecHandle StaminaDrainEffectSpecHandle;
	
	
};
