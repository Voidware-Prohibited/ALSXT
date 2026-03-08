// MIT


#include "AbilitySystem/GameplayAbilities/AlsxtGameplayAbilityAim.h"

#include "AlsxtCharacter.h"


UAlsxtGameplayAbilityAim::UAlsxtGameplayAbilityAim()
{
	FGameplayTagContainer AssetTags = { };
	AssetTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Gameplay.Ability.Aim")));
	SetAssetTags(AssetTags);
}

void UAlsxtGameplayAbilityAim::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// AAlsxtCharacter* Character = Cast<AAlsxtCharacter>(ActorInfo->AvatarActor.Get());

	if (GetAvatarActorFromActorInfo()->Implements<UAlsxtCharacterInterface>())
	{
		if (IAlsxtCharacterInterface::Execute_GetCanCharacterAim(GetAvatarActorFromActorInfo()))
		{
			IAlsxtCharacterInterface::Execute_SetCharacterRotationMode(GetAvatarActorFromActorInfo(), AlsRotationModeTags::Aiming);
			// Character->SetDesiredRotationMode(AlsRotationModeTags::Aiming);
			// Character->SetDesiredAiming(true);
			IAlsxtCharacterInterface::Execute_SetCharacterAim(GetAvatarActorFromActorInfo(), true);
			IAlsxtCharacterInterface::Execute_SetCharacterReadiness(GetAvatarActorFromActorInfo(), AlsxtReadinessTags::Aiming);
		}
	}
	
}

void UAlsxtGameplayAbilityAim::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	if (ScopeLockCount > 0)
	{
		WaitingToExecute.Add(FPostLockDelegate::CreateUObject(this, &UAlsxtGameplayAbilityAim::CancelAbility, Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility));
		return;
	}
	
	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);

	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UAlsxtGameplayAbilityAim::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (!IsEndAbilityValid(Handle, ActorInfo))
	{
		return;
	}

	if (ScopeLockCount > 0)
	{
		WaitingToExecute.Add(FPostLockDelegate::CreateUObject(this, &UAlsxtGameplayAbilityAim::EndAbility, Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled));
		return;
	}

	if (GetAvatarActorFromActorInfo()->Implements<UAlsxtCharacterInterface>())
	{
		// AAlsxtCharacter* Character = Cast<AAlsxtCharacter>(ActorInfo->AvatarActor.Get());
		// Character->SetDesiredRotationMode(AlsRotationModeTags::ViewDirection);
		IAlsxtCharacterInterface::Execute_SetCharacterRotationMode(GetAvatarActorFromActorInfo(), AlsRotationModeTags::ViewDirection);
		// Character->SetDesiredAiming(false);
		IAlsxtCharacterInterface::Execute_SetCharacterAim(GetAvatarActorFromActorInfo(), false);
		IAlsxtCharacterInterface::Execute_SetCharacterReadiness(GetAvatarActorFromActorInfo(), AlsxtReadinessTags::Ready);
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);	
}

void UAlsxtGameplayAbilityAim::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
}

bool UAlsxtGameplayAbilityAim::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                                  const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (GetAvatarActorFromActorInfo()->Implements<UAlsxtCharacterInterface>())
	{
		return IAlsxtCharacterInterface::Execute_GetCanCharacterAim(GetAvatarActorFromActorInfo());
	}
	return false;
}
