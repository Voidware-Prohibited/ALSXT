// MIT


#include "AbilitySystem/GameplayAbilities/AlsxtGameplayAbilityTargeting.h"

#include "AbilitySystem/AbilityTasks/AlsxtAbilityTaskPerformTargeting.h"

void UAlsxtGameplayAbilityTargeting::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		return;
	}

	// Use the custom targeting task
	UAlsxtAbilityTaskPerformTargeting* TargetingTask = UAlsxtAbilityTaskPerformTargeting::CustomTargetDataTask(this, FName("CustomTargetingTask"));
	if (TargetingTask)
	{
		// Bind the callback function to the delegate
		TargetingTask->OnTargetDataReady.AddDynamic(this, &UAlsxtGameplayAbilityTargeting::OnTargetDataReadyCallback);
		TargetingTask->Activate();
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}
}

void UAlsxtGameplayAbilityTargeting::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	// Process the DataHandle here (e.g., apply effects to targets)
	// You can iterate through the DataHandle and apply a Gameplay Effect Spec Handle

	// Example: Apply a dummy gameplay effect to the first target hit
	if (DataHandle.IsValid(0))
	{
		// Get the target actor from the data handle
		// AActor* TargetActor = DataHandle;
		// if (IsValid(TargetActor))
		// {
		// 	// You would get your effect class and apply it here
		// 	// MakeGameplayEffectSpecHandle(...);
		// 	// ApplyGameplayEffectSpecToTarget(...);
		// }
	}// 

	// End the ability when done
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

