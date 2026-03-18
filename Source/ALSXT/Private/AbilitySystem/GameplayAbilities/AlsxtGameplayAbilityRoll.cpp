// MIT


#include "AbilitySystem/GameplayAbilities/AlsxtGameplayAbilityRoll.h"

#include "AlsxtCharacter.h"
#include "AbilitySystem/AttributeSets/AlsxtStaminaAttributeSet.h"


UAlsxtGameplayAbilityRoll::UAlsxtGameplayAbilityRoll()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	FGameplayTagContainer AssetTags = { };
	AssetTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Gameplay.Ability.Roll")));
	SetAssetTags(AssetTags);

	// Default tag for setting the stamina cost magnitude
	StaminaCostTag = FGameplayTag::RequestGameplayTag(FName("StaminaCost.Instant.Roll"));
}

void UAlsxtGameplayAbilityRoll::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!ActorInfo->IsNetAuthority())
	{
		return;
	}

	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		}
		
		if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
		{
			// if (AAlsxtCharacter* Character = Cast<AAlsxtCharacter>(ActorInfo->AvatarActor.Get()))
			// {
			// 	if (CostGameplayEffectClass)
			// 	{
			// 		FGameplayEffectContextHandle EffectContext = ActorInfo->AbilitySystemComponent->MakeEffectContext();
			// 		EffectContext.AddInstigator(ActorInfo->AvatarActor.Get(), ActorInfo->AvatarActor.Get());
			// 		StaminaDrainEffectSpecHandle = ActorInfo->AbilitySystemComponent->MakeOutgoingSpec(CostGameplayEffectClass, 1.0f, EffectContext);
			// 		// StaminaDrainEffectSpecHandle.Data->SetSetByCallerMagnitude(StaminaCostTag, -BaseStaminaCostPerSecond);
        // 
			// 		if (StaminaDrainEffectSpecHandle.IsValid())
			// 		{
			// 			ActorInfo->AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*StaminaDrainEffectSpecHandle.Data.Get());
			// 		}
			// 	}
			// 	
			// 	Character->StartRolling(1.3f);
// 
			// 	// Cancel Stamina Regen Ability
			// 	FGameplayTagContainer CancelStaminaRegenGameplayContainer;
			// 	CancelStaminaRegenGameplayContainer.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Gameplay.Ability.StaminaRegen")));
			// 	ActorInfo->AbilitySystemComponent->CancelAbilities(&CancelStaminaRegenGameplayContainer);
// 
			// 	// EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
			// }

			if (GetAvatarActorFromActorInfo()->Implements<UAlsxtCharacterInterface>())
			{
				if (CostGameplayEffectClass)
				{
					FGameplayEffectContextHandle EffectContext = ActorInfo->AbilitySystemComponent->MakeEffectContext();
					EffectContext.AddInstigator(ActorInfo->AvatarActor.Get(), ActorInfo->AvatarActor.Get());
					StaminaDrainEffectSpecHandle = ActorInfo->AbilitySystemComponent->MakeOutgoingSpec(CostGameplayEffectClass, 1.0f, EffectContext);
					// StaminaDrainEffectSpecHandle.Data->SetSetByCallerMagnitude(StaminaCostTag, -BaseStaminaCostPerSecond);
        
					if (StaminaDrainEffectSpecHandle.IsValid())
					{
						ActorInfo->AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*StaminaDrainEffectSpecHandle.Data.Get());
					}
				}

				IAlsxtCharacterInterface::Execute_StartCharacterRoll(GetAvatarActorFromActorInfo(), 1.3f);

				// Cancel Stamina Regen Ability
				FGameplayTagContainer CancelStaminaRegenGameplayContainer;
				CancelStaminaRegenGameplayContainer.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Gameplay.Ability.StaminaRegen")));
				ActorInfo->AbilitySystemComponent->CancelAbilities(&CancelStaminaRegenGameplayContainer);
			}
		}
	}
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UAlsxtGameplayAbilityRoll::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	if (ScopeLockCount > 0)
	{
		WaitingToExecute.Add(FPostLockDelegate::CreateUObject(this, &UAlsxtGameplayAbilityRoll::CancelAbility, Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility));
		return;
	}
	
	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);

	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UAlsxtGameplayAbilityRoll::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// Apply Cooldown (which handles re-activating later)
	if (ActorInfo && ActorInfo->AbilitySystemComponent.IsValid())
	{
		if (GetCooldownGameplayEffect())
		{
			FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(GetCooldownGameplayEffect()->GetClass(), 1.0f);
        
			if (SpecHandle.IsValid())
			{
				// Apply Cooldown Spec to Self
				GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
		}
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UAlsxtGameplayAbilityRoll::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	if (!ASC)
	{
		return false;
	}

	// Get the Stamina Attribute Set
	const UAlsxtStaminaAttributeSet* StaminaSet = ASC->GetSet<UAlsxtStaminaAttributeSet>();
	if (!StaminaSet)
	{
		return false;
	}

	// Calculate the jump cost from the Gameplay Effect
	
	// const AAlsxtCharacter* Character = CastChecked<AAlsxtCharacter>(ActorInfo->AvatarActor.Get(), ECastCheckedType::NullAllowed);
	float CurrentStamina = StaminaSet->GetCurrentStamina();

	if (GetAvatarActorFromActorInfo()->Implements<UAlsxtCharacterInterface>())
	{
		if (CostGameplayEffectClass)
		{
			FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
			FGameplayEffectSpecHandle CostSpecHandle = ASC->MakeOutgoingSpec(CostGameplayEffectClass, 1.0f, EffectContext);
			if (CostSpecHandle.IsValid())
			{
				for (const FGameplayModifierInfo& Modifier : CostSpecHandle.Data->Def->Modifiers)
				{
					if (Modifier.Attribute == UAlsxtStaminaAttributeSet::GetCurrentStaminaAttribute() && Modifier.ModifierOp == EGameplayModOp::Additive)
					{
						float RollCost = 0.01f;
						RollCost -= CostSpecHandle.Data->GetModifierMagnitude(0);
						return CurrentStamina >= FMath::Abs(RollCost) && IAlsxtCharacterInterface::Execute_GetCharacterCanRoll(GetAvatarActorFromActorInfo()); // Use FMath::Abs as cost is likely negative
					}
				}
			}
			return IAlsxtCharacterInterface::Execute_GetCharacterCanRoll(GetAvatarActorFromActorInfo());
			// return Character->CanRoll();
		}
		return IAlsxtCharacterInterface::Execute_GetCharacterCanRoll(GetAvatarActorFromActorInfo());
	}
	return false;
	
	/// return Character->CanRoll();
}

void UAlsxtGameplayAbilityRoll::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (ActorInfo != NULL && ActorInfo->AvatarActor != NULL)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}
}
