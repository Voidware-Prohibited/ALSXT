// MIT


#include "AbilitySystem/GameplayAbilities/AlsxtGameplayAbilityFreelook.h"

#include "Interfaces/AlsxtCharacterInterface.h"

void UAlsxtGameplayAbilityFreelook::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                                    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	IAlsxtCharacterInterface::Execute_CharacterActivateFreelooking(GetAvatarActorFromActorInfo());
}

void UAlsxtGameplayAbilityFreelook::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	IAlsxtCharacterInterface::Execute_CharacterDeactivateFreelooking(GetAvatarActorFromActorInfo());
}
