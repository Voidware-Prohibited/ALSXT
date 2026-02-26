// MIT


#include "AbilitySystem/GameplayAbilities/AlsxtGameplayAbilityExternalCooldowns.h"
#include "AbilitySystem/AbilitySystemComponent/AlsxtAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

void UAlsxtGameplayAbilityExternalCooldowns::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ApplyExternalCooldownEffects(OnAbilityActivatedCooldownEffects);
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UAlsxtGameplayAbilityExternalCooldowns::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ApplyExternalCooldownEffects(OnAbilityEndedCooldownEffects);
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UAlsxtGameplayAbilityExternalCooldowns::ApplyExternalCooldownEffects(TArray<FAlsxtCooldownEffectEntry> CooldownEffects)
{
	if (CooldownEffects.IsEmpty())
	{
		return;
	}
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	
	if (ASC)
	{
		for (FAlsxtCooldownEffectEntry CooldownEffect : CooldownEffects)
		{
			FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(CooldownEffect.CooldownGameplayEffectClass, 1.f, ASC->MakeEffectContext());
			if (SpecHandle.IsValid())
			{
				// Assign the magnitude value to the specific tag using SetByCaller
				UAbilitySystemBlueprintLibrary::AssignSetByCallerMagnitude(SpecHandle, CooldownEffect.MagnitudeTag.GetTagName(), CooldownEffect.Magnitude);
                
				// Apply the effect
				ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
		}
	}
	
}
