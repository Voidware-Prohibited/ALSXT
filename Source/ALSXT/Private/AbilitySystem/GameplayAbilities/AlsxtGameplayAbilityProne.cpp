// MIT

#include "AbilitySystem/GameplayAbilities/AlsxtGameplayAbilityProne.h"
#include "AbilitySystem/AbilitySystemComponent/AlsxtAbilitySystemComponent.h"
#include "AlsxtCharacter.h"
#include "AlsxtCharacterMovementComponent.h"
#include "AbilitySystem/AttributeSets/AlsxtStaminaAttributeSet.h"
#include "GameplayEffect.h"
#include "TimerManager.h"

UAlsxtGameplayAbilityProne::UAlsxtGameplayAbilityProne()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	FGameplayTagContainer AssetTags = { };
	AssetTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Gameplay.Ability.Running")));
	// AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Gameplay.Ability.Sprint")));
	SetAssetTags(AssetTags);
	BaseStaminaCostPerSecond = 0.05f;
	StaminaCostTag = FGameplayTag::RequestGameplayTag(FName("StaminaCost.Infinite.Running"));
}

void UAlsxtGameplayAbilityProne::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
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

	if (StaminaCostEffect)
	{
		FGameplayEffectContextHandle EffectContext = ActorInfo->AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddInstigator(ActorInfo->AvatarActor.Get(), ActorInfo->AvatarActor.Get());
		StaminaDrainEffectSpecHandle = ActorInfo->AbilitySystemComponent->MakeOutgoingSpec(StaminaCostEffect, 1.0f, EffectContext);
		StaminaDrainEffectSpecHandle.Data->SetSetByCallerMagnitude(StaminaCostTag, -BaseStaminaCostPerSecond);
        
		if (StaminaDrainEffectSpecHandle.IsValid())
		{
			ActorInfo->AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*StaminaDrainEffectSpecHandle.Data.Get());
		}
	}

	AAlsxtCharacter* Character = Cast<AAlsxtCharacter>(ActorInfo->AvatarActor.Get());
	Character->SetDesiredGait(AlsGaitTags::Running);
}

void UAlsxtGameplayAbilityProne::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (!IsEndAbilityValid(Handle, ActorInfo))
	{
		return;
	}

	if (ScopeLockCount > 0)
	{
		WaitingToExecute.Add(FPostLockDelegate::CreateUObject(this, &UAlsxtGameplayAbilityProne::EndAbility, Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled));
		return;
	}

	// Remove the stamina drain Gameplay Effect
	if (HasAuthority(&ActivationInfo) && StaminaDrainEffectSpecHandle.IsValid())
	{
		ActorInfo->AbilitySystemComponent->RemoveActiveGameplayEffectBySourceEffect(StaminaCostEffect, ActorInfo->AbilitySystemComponent.Get(), -1);
	}

	
	// if (HasAuthority(&ActivationInfo) && ActiveStaminaDrainEffectHandle.IsValid())
	// {
	// 	ActorInfo->AbilitySystemComponent->RemoveActiveGameplayEffect(ActiveStaminaDrainEffectHandle);
	// }
	

	AAlsxtCharacter* Character = Cast<AAlsxtCharacter>(ActorInfo->AvatarActor.Get());
	Character->SetDesiredGait(AlsGaitTags::Walking);

	// Try to Activate Stamina Regen Ability
	if (StaminaCostEffect)
	{
		FGameplayTagContainer StaminaRegenGameplayTags;
		StaminaRegenGameplayTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Gameplay.Ability.StaminaRegen")));
		if (ActorInfo->AbilitySystemComponent->TryActivateAbilitiesByTag(StaminaRegenGameplayTags, true))
		{
			UE_LOG(LogTemp, Warning, TEXT("UAlsxtGameplayAbilitySprint::EndAbility: Gameplay.Ability.StaminaRegen activated!"));
		}
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UAlsxtGameplayAbilityProne::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
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
