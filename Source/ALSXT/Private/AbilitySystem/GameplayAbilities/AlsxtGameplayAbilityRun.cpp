// MIT

#include "AbilitySystem/GameplayAbilities/AlsxtGameplayAbilityRun.h"
#include "AbilitySystem/AbilitySystemComponent/AlsxtAbilitySystemComponent.h"
#include "AlsxtCharacter.h"
#include "AlsxtCharacterMovementComponent.h"
#include "AbilitySystem/AttributeSets/AlsxtStaminaAttributeSet.h"
#include "GameplayEffect.h"
#include "TimerManager.h"

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

	AAlsxtCharacter* Character = Cast<AAlsxtCharacter>(ActorInfo->AvatarActor.Get());

	if (Character->GetDesiredGait() == AlsGaitTags::Walking)
	{
		Character->SetDesiredGait(AlsGaitTags::Running);

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
	}
	else
	{
		if (Character->GetDesiredGait() == AlsGaitTags::Running)
		{
			// Character->SetDesiredGait(AlsGaitTags::Walking);
			EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		}
	}
}

void UAlsxtGameplayAbilityRun::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	if (ScopeLockCount > 0)
	{
		WaitingToExecute.Add(FPostLockDelegate::CreateUObject(this, &UAlsxtGameplayAbilityRun::CancelAbility, Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility));
		return;
	}
	
	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);

	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
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

	AAlsxtCharacter* Character = Cast<AAlsxtCharacter>(ActorInfo->AvatarActor.Get());
	Character->SetDesiredGait(AlsGaitTags::Walking);

	// Try to Activate Stamina Regen Ability
	// if (CostGameplayEffectClass)
	// {
	// 	FGameplayTagContainer StaminaRegenGameplayTags;
	// 	StaminaRegenGameplayTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Gameplay.Ability.StaminaRegen")));
	// 	if (ActorInfo->AbilitySystemComponent->TryActivateAbilitiesByTag(StaminaRegenGameplayTags, true))
	// 	{
	// 		UE_LOG(LogTemp, Warning, TEXT("UAlsxtGameplayAbilityRun::EndAbility: Gameplay.Ability.StaminaRegen activated!"));
	// 	}
	// }

	// Try to Activate Stamina Regen Ability
	FGameplayTagContainer StaminaRegenGameplayTags;
	StaminaRegenGameplayTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Gameplay.Ability.StaminaRegen")));
	if (ActorInfo->AbilitySystemComponent->TryActivateAbilitiesByTag(StaminaRegenGameplayTags, true))
	{
		UE_LOG(LogTemp, Warning, TEXT("UAlsxtGameplayAbilityRun::EndAbility: Gameplay.Ability.StaminaRegen activated!"));
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
		// Optionally, provide a reason why activation failed for debugging.
		// You can use a tag, for example: FGameplayTag::RequestGameplayTag(TEXT("Ability.Cost.MissingAttributeSet"));
		return false;
	}

	
	
	// Calculate the jump cost from the Gameplay Effect
	
	AAlsxtCharacter* Character = Cast<AAlsxtCharacter>(ActorInfo->AvatarActor.Get());

	return (Character->GetDesiredGait() == AlsGaitTags::Walking) || (Character->GetDesiredGait() == AlsGaitTags::Sprinting);

	return true;
	
	float CurrentStamina = StaminaAttributeSet->GetCurrentStamina();
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
					float JumpCost = 0.0f;
					JumpCost -= CostSpecHandle.Data->GetModifierMagnitude(0);
					return CurrentStamina >= FMath::Abs(JumpCost); // Use FMath::Abs as cost is likely negative
				}
			}
		}
		return false;
	}
	return false;
}
