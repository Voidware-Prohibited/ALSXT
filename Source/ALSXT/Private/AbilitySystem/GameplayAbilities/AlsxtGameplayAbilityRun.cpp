// Copyright (C) 2025 Uriel Ballinas, VOIDWARE Prohibited. All rights reserved.
// This software is licensed under the MIT License (LICENSE.md).

#include "AbilitySystem/GameplayAbilities/AlsxtGameplayAbilityRun.h"
#include "AbilitySystem/AbilitySystemComponent/AlsxtAbilitySystemComponent.h"
#include "AlsxtCharacter.h"
#include "AlsxtCharacterMovementComponent.h"
#include "AbilitySystem/AttributeSets/AlsxtStaminaAttributeSet.h"
#include "GameplayEffect.h"
#include "TimerManager.h"

/**
* @file AlsxtGameplayAbilityRun.cpp
* @brief Gameplay Ability for toggling between Walking and Running/Combat Gaits
*/

UAlsxtGameplayAbilityRun::UAlsxtGameplayAbilityRun()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	FGameplayTagContainer AssetTags = { };
	AssetTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Gameplay.Ability.Running")));
	SetAssetTags(AssetTags);
	StaminaCostTag = FGameplayTag::RequestGameplayTag(FName("StaminaCost.Infinite.Running"));
}

void UAlsxtGameplayAbilityRun::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
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

	// If Current Overlay allows for Combat Gait, use that instead
	if (GetAvatarActorFromActorInfo()->Implements<UAlsxtCharacterInterface>())
	{
		if (IAlsxtCharacterInterface::Execute_GetCharacterAvailableGaits(GetAvatarActorFromActorInfo()).HasTag(AlsGaitTags::Combat))
		{
			IAlsxtCharacterInterface::Execute_SetCharacterGait(GetAvatarActorFromActorInfo(), AlsGaitTags::Combat);
		}
		else
		{
			IAlsxtCharacterInterface::Execute_SetCharacterGait(GetAvatarActorFromActorInfo(), AlsGaitTags::Running);
		}
	}
}

void UAlsxtGameplayAbilityRun::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                          const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (!IsEndAbilityValid(Handle, ActorInfo))
	{
		return;
	}

	if (ScopeLockCount > 0)
	{
		WaitingToExecute.Add(FPostLockDelegate::CreateUObject(this, &UAlsxtGameplayAbilityRun::EndAbility, Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled));
		return;
	}

	// Remove the stamina drain Gameplay Effect
	if (HasAuthority(&ActivationInfo) && StaminaDrainEffectSpecHandle.IsValid())
	{
		ActorInfo->AbilitySystemComponent->RemoveActiveGameplayEffectBySourceEffect(CostGameplayEffectClass, ActorInfo->AbilitySystemComponent.Get(), -1);
	}

	if (GetAvatarActorFromActorInfo()->Implements<UAlsxtCharacterInterface>())
	{
		IAlsxtCharacterInterface::Execute_SetCharacterGait(GetAvatarActorFromActorInfo(), AlsGaitTags::Walking);
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UAlsxtGameplayAbilityRun::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	if (!IsValid(ASC))
	{
		return false;
	}
	
	const UAlsxtStaminaAttributeSet* StaminaAttributeSet = ASC->GetSet<UAlsxtStaminaAttributeSet>();
	
	if (!IsValid(StaminaAttributeSet))
	{
		return false;
	}
	
	// if (GetAvatarActorFromActorInfo()->Implements<UAlsxtCharacterInterface>())
	// {
	// 	return !(IAlsxtCharacterInterface::Execute_GetCharacterGait(GetAvatarActorFromActorInfo()) == AlsGaitTags::Running) || !(IAlsxtCharacterInterface::Execute_GetCharacterGait(GetAvatarActorFromActorInfo()) == AlsGaitTags::Combat);
	// }
	// return false;

	return true;
}
