// MIT

#pragma once

#include "CoreMinimal.h"
#include "AlsxtGameplayAbilityBase.h"
#include "GameplayBehaviorSmartObjectBehaviorDefinition.h"
#include "Abilities/GameplayAbility.h"
#include "SmartObjectSubsystem.h"
#include "AlsxtGameplayAbilityInteraction.generated.h"

/**
 * 
 */
UCLASS()
class ALSXT_API UAlsxtGameplayAbilityInteraction : public UAlsxtGameplayAbilityBase
{
	GENERATED_BODY()

public:
	UAlsxtGameplayAbilityInteraction();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnTargetFound(FSmartObjectHandle SmartObjectHandle, FSmartObjectSlotHandle SlotHandle, FSmartObjectClaimHandle ClaimHandle);

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	/** The behavior definition we are looking to execute on the smart object */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smart Object")
	TSubclassOf<UGameplayBehaviorSmartObjectBehaviorDefinition> BehaviorDefinitionClass;

	/** Handle to the currently claimed slot */
	FSmartObjectClaimHandle CurrentClaimHandle;

	void ReleaseSlot();
};
