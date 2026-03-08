// MIT


#include "AbilitySystem/GameplayAbilities/AlsxtGameplayAbilitySwitchCombatStance.h"

#include "AlsxtCharacter.h"
#include "Utility/AlsxtGameplayTags.h"


UAlsxtGameplayAbilitySwitchCombatStance::UAlsxtGameplayAbilitySwitchCombatStance()
{
	FGameplayTagContainer AssetTags = { };
	AssetTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Gameplay.Ability.SwitchCombatStance")));
	SetAssetTags(AssetTags);
}

void UAlsxtGameplayAbilitySwitchCombatStance::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	AAlsxtCharacter* Character = Cast<AAlsxtCharacter>(ActorInfo->AvatarActor.Get());
	if (Character->Implements<UAlsxtCharacterInterface>() && Character->Implements<UAlsxtCharacterGameplayCameraComponentInterface>())
	{
		// if (IAlsxtCharacterGameplayCameraComponentInterface::Execute_CanCharacterSwitchCameraRightShoulder(Character))
		// {
		// 				
		// }
		
		if (IAlsxtCharacterInterface::Execute_CanCharacterSwitchCombatStance(Character))
		{
			if (IAlsxtCharacterGameplayCameraComponentInterface::Execute_GetCharacterIsCameraRightShoulder(Character))
			{
				IAlsxtCharacterGameplayCameraComponentInterface::Execute_SetCharacterCameraRightShoulder(Character, false);
				IAlsxtCharacterInterface::Execute_SetCharacterCombatStance(Character, ALSXTCombatStanceTags::Southpaw);
			}
			else
			{
				IAlsxtCharacterGameplayCameraComponentInterface::Execute_SetCharacterCameraRightShoulder(Character, true);
				IAlsxtCharacterInterface::Execute_SetCharacterCombatStance(Character, ALSXTCombatStanceTags::Orthodox);
			}
		}
										
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UAlsxtGameplayAbilitySwitchCombatStance::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UAlsxtGameplayAbilitySwitchCombatStance::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UAlsxtGameplayAbilitySwitchCombatStance::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}
