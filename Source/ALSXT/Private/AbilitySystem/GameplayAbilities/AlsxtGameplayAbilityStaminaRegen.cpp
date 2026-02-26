// MIT


#include "AbilitySystem/GameplayAbilities/AlsxtGameplayAbilityStaminaRegen.h"
#include "AbilitySystem/AbilitySystemComponent/AlsxtAbilitySystemComponent.h"
#include "AlsxtCharacter.h"
#include "AlsxtCharacterMovementComponent.h"
#include "AbilitySystem/AttributeSets/AlsxtStaminaAttributeSet.h"
#include "GameplayEffect.h"

UAlsxtGameplayAbilityStaminaRegen::UAlsxtGameplayAbilityStaminaRegen()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	FGameplayTagContainer AssetTags = { };
	AssetTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Gameplay.Ability.StaminaRegen")));
	SetAssetTags(AssetTags);
	BaseStaminaRegenPerSecond = 0.05f;
	StaminaRegenTag = FGameplayTag::RequestGameplayTag(FName("StaminaCost.Infinite.Sprint"));

	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Consuming.Stamina")));
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Cooldown.StaminaRegen")));
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Max.Stamina")));

	FGameplayTagContainer BlockTags;
	BlockTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Consuming.Stamina")));
	BlockTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Cooldown.StaminaRegen")));
	BlockTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Max.Stamina")));
	ActivationBlockedTags = BlockTags;
}

void UAlsxtGameplayAbilityStaminaRegen::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
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

	FGameplayEffectContextHandle EffectContext = GetActorInfo().AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddInstigator(GetActorInfo().AvatarActor.Get(), GetActorInfo().AvatarActor.Get());
	StaminaRegenEffectSpecHandle = GetActorInfo().AbilitySystemComponent->MakeOutgoingSpec(StaminaRegenEffect, 1.0f, EffectContext);
	StaminaRegenEffectSpecHandle.Data->SetSetByCallerMagnitude(StaminaRegenTag, -BaseStaminaRegenPerSecond);
        
	if (StaminaRegenEffectSpecHandle.IsValid())
	{
		GetActorInfo().AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*StaminaRegenEffectSpecHandle.Data.Get());
	}
}

void UAlsxtGameplayAbilityStaminaRegen::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (!IsEndAbilityValid(Handle, ActorInfo))
	{
		return;
	}

	if (ScopeLockCount > 0)
	{
		WaitingToExecute.Add(FPostLockDelegate::CreateUObject(this, &UAlsxtGameplayAbilityStaminaRegen::EndAbility, Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled));
		return;
	}

	// Remove the stamina drain Gameplay Effect
	// if (HasAuthority(&ActivationInfo) && StaminaRegenEffectSpecHandle.IsValid())
	// {
	// 	ActorInfo->AbilitySystemComponent->RemoveActiveGameplayEffectBySourceEffect(StaminaRegenEffect, ActorInfo->AbilitySystemComponent.Get(), -1);
	// }
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UAlsxtGameplayAbilityStaminaRegen::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
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
	
	return true;
}
