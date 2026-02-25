// MIT


#include "AbilitySystem/GameplayAbilities/AlsxtGameplayAbilityAim.h"

#include "AlsxtCharacter.h"


UAlsxtGameplayAbilityAim::UAlsxtGameplayAbilityAim()
{
}

void UAlsxtGameplayAbilityAim::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	AAlsxtCharacter* Character = Cast<AAlsxtCharacter>(ActorInfo->AvatarActor.Get());
	if (Character->CanAim())
	{
		Character->SetDesiredRotationMode(AlsRotationModeTags::Aiming);
	}
}

void UAlsxtGameplayAbilityAim::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
}

void UAlsxtGameplayAbilityAim::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	AAlsxtCharacter* Character = Cast<AAlsxtCharacter>(ActorInfo->AvatarActor.Get());
	Character->SetDesiredRotationMode(AlsRotationModeTags::ViewDirection);
}

bool UAlsxtGameplayAbilityAim::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (AAlsxtCharacter* Character = Cast<AAlsxtCharacter>(ActorInfo->AvatarActor.Get()))
	{
		return Character->CanAim();
	}
	return false;
}
