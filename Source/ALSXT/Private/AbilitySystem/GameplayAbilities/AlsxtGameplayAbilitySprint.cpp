// MIT

#include "AbilitySystem/GameplayAbilities/AlsxtGameplayAbilitySprint.h"
#include "AbilitySystem/AbilitySystemComponent/AlsxtAbilitySystemComponent.h"
#include "AlsxtCharacter.h"
#include "AlsxtCharacterMovementComponent.h"
#include "AbilitySystem/AttributeSets/AlsxtStaminaAttributeSet.h"
#include "GameplayEffect.h"
#include "TimerManager.h"

UAlsxtGameplayAbilitySprint::UAlsxtGameplayAbilitySprint()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	FGameplayTagContainer AssetTags = { };
	AssetTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Gameplay.Ability.Sprint")));
	SetAssetTags(AssetTags);
	StaminaCostTag = FGameplayTag::RequestGameplayTag(FName("StaminaCost.Infinite.Sprint"));
}

void UAlsxtGameplayAbilitySprint::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
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

	// Cancel Stamina Regen Ability
	FGameplayTagContainer CancelStaminaRegenGameplayContainer;
	CancelStaminaRegenGameplayContainer.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Gameplay.Ability.StaminaRegen")));
	ActorInfo->AbilitySystemComponent->CancelAbilities(&CancelStaminaRegenGameplayContainer);

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

	// AAlsxtCharacter* Character = Cast<AAlsxtCharacter>(ActorInfo->AvatarActor.Get());
	// PreviousGait = Character->GetDesiredGait();
	// 
	// if (Character->CanSprint())
	// {
	// 	Character->SetDesiredGait(AlsGaitTags::Sprinting);
	// }

	if (GetAvatarActorFromActorInfo()->Implements<UAlsxtCharacterInterface>())
	{
		PreviousGait = IAlsxtCharacterInterface::Execute_GetCharacterGait(GetAvatarActorFromActorInfo());
		if (IAlsxtCharacterInterface::Execute_CanCharacterSprint(GetAvatarActorFromActorInfo()))
		{
			IAlsxtCharacterInterface::Execute_SetCharacterGait(GetAvatarActorFromActorInfo(), AlsGaitTags::Sprinting);
		}
		
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UAlsxtGameplayAbilitySprint::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	if (ScopeLockCount > 0)
	{
		WaitingToExecute.Add(FPostLockDelegate::CreateUObject(this, &UAlsxtGameplayAbilitySprint::CancelAbility, Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility));
		return;
	}
	
	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);

	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UAlsxtGameplayAbilitySprint::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                             const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (!IsEndAbilityValid(Handle, ActorInfo))
	{
		return;
	}

	if (ScopeLockCount > 0)
	{
		WaitingToExecute.Add(FPostLockDelegate::CreateUObject(this, &UAlsxtGameplayAbilitySprint::EndAbility, Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled));
		return;
	}

	// Remove the stamina drain Gameplay Effect
	if (HasAuthority(&ActivationInfo) && StaminaDrainEffectSpecHandle.IsValid())
	{
		ActorInfo->AbilitySystemComponent->RemoveActiveGameplayEffectBySourceEffect(CostGameplayEffectClass, ActorInfo->AbilitySystemComponent.Get(), -1);
	}

	// AAlsxtCharacter* Character = Cast<AAlsxtCharacter>(ActorInfo->AvatarActor.Get());
	// Character->SetDesiredGait(PreviousGait);

	if (GetAvatarActorFromActorInfo()->Implements<UAlsxtCharacterInterface>())
	{
		IAlsxtCharacterInterface::Execute_SetCharacterGait(GetAvatarActorFromActorInfo(), PreviousGait);
	}

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

bool UAlsxtGameplayAbilitySprint::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
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
		// Optionally, provide a reason why activation failed for debugging.
		// You can use a tag, for example: FGameplayTag::RequestGameplayTag(TEXT("Ability.Cost.MissingAttributeSet"));
		return false;
	}
	
	// Calculate the jump cost from the Gameplay Effect
	
	const ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get(), ECastCheckedType::NullAllowed);
	float CurrentStamina = StaminaAttributeSet->GetCurrentStamina();
	//if (CostGameplayEffectClass)
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
					float InitialCost = 0.0f;
					InitialCost -= CostSpecHandle.Data->GetModifierMagnitude(0);
					return CurrentStamina >= FMath::Abs(InitialCost); // Use FMath::Abs as cost is likely negative
				}
			}
		}
		return false;
	}
	return false;
}
