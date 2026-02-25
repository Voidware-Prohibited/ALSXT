// MIT


#include "AbilitySystem/GameplayAbilities/AlsxtGameplayAbilityFocus.h"
#include "AbilitySystem/AbilitySystemComponent/AlsxtAbilitySystemComponent.h"
#include "AlsxtCharacter.h"
#include "GameplayEffect.h"
#include "TimerManager.h"

UAlsxtGameplayAbilityFocus::UAlsxtGameplayAbilityFocus()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	FGameplayTagContainer AssetTags = { };
	AssetTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Gameplay.Ability.Focus")));
	SetAssetTags(AssetTags);
}

void UAlsxtGameplayAbilityFocus::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		return;
	}

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (ActorInfo->AvatarActor.Get()->Implements<UAlsxtCharacterInterface>())
	{
		IAlsxtCharacterInterface::Execute_SetCharacterFocus(ActorInfo->AvatarActor.Get(), true);
	}
}

void UAlsxtGameplayAbilityFocus::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	if (ScopeLockCount > 0)
	{
		WaitingToExecute.Add(FPostLockDelegate::CreateUObject(this, &UAlsxtGameplayAbilityFocus::CancelAbility, Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility));
		return;
	}
	
	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);

	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UAlsxtGameplayAbilityFocus::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                             const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (!IsEndAbilityValid(Handle, ActorInfo))
	{
		return;
	}

	if (ScopeLockCount > 0)
	{
		WaitingToExecute.Add(FPostLockDelegate::CreateUObject(this, &UAlsxtGameplayAbilityFocus::EndAbility, Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled));
		return;
	}

	if (ActorInfo->AvatarActor.Get()->Implements<UAlsxtCharacterInterface>())
	{
		IAlsxtCharacterInterface::Execute_SetCharacterFocus(ActorInfo->AvatarActor.Get(), false);
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UAlsxtGameplayAbilityFocus::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	if (!IsValid(ASC))
	{
		return false;
	}

	if (ActorInfo->AvatarActor.Get()->Implements<UAlsxtCharacterInterface>())
	{
		return !IAlsxtCharacterInterface::Execute_GetCharacterFocus(ActorInfo->AvatarActor.Get());
	}
		return false;
}

void UAlsxtGameplayAbilityFocus::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
}


