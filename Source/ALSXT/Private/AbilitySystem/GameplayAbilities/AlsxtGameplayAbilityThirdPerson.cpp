// MIT


#include "AbilitySystem/GameplayAbilities/AlsxtGameplayAbilityThirdPerson.h"

#include "AlsxtCharacter.h"


UAlsxtGameplayAbilityThirdPerson::UAlsxtGameplayAbilityThirdPerson()
{
}

void UAlsxtGameplayAbilityThirdPerson::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	AAlsxtCharacter* Character = Cast<AAlsxtCharacter>(ActorInfo->AvatarActor.Get());
	Character->SetViewMode(AlsViewModeTags::ThirdPerson);
}

void UAlsxtGameplayAbilityThirdPerson::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
}

void UAlsxtGameplayAbilityThirdPerson::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{	
	if (!IsEndAbilityValid(Handle, ActorInfo))
    {
    	return;
    }
    
    if (ScopeLockCount > 0)
    {
    	WaitingToExecute.Add(FPostLockDelegate::CreateUObject(this, &UAlsxtGameplayAbilityThirdPerson::EndAbility, Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled));
    	return;
    }

	AAlsxtCharacter* Character = Cast<AAlsxtCharacter>(ActorInfo->AvatarActor.Get());
    Character->SetViewMode(AlsViewModeTags::FirstPerson);
    
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UAlsxtGameplayAbilityThirdPerson::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	if (!ASC)
	{
		return false;
	}
	
	AAlsxtCharacter* Character = Cast<AAlsxtCharacter>(ActorInfo->AvatarActor.Get());
	
	return Character->GetViewMode() == AlsViewModeTags::FirstPerson;
}
