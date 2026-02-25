// MIT


#include "AbilitySystem/GameplayAbilities/AlsxtGameplayAbilityFlip.h"
#include "AbilitySystem/AbilitySystemComponent/AlsxtAbilitySystemComponent.h"
#include "AlsxtCharacter.h"
#include "AbilitySystem/AttributeSets/AlsxtStaminaAttributeSet.h"
#include "GameFramework/Character.h"

UAlsxtGameplayAbilityFlip::UAlsxtGameplayAbilityFlip()
{
	// InstancingPolicy = EGameplayAbilityInstancingPolicy::NonInstanced;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	FGameplayTagContainer AssetTags = { };
	AssetTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Gameplay.Ability.Flip")));
	SetAssetTags(AssetTags);

	// Default tag for setting the stamina cost magnitude
	StaminaCostTag = FGameplayTag::RequestGameplayTag(FName("StaminaCost.Instant.Flip"));

}

void UAlsxtGameplayAbilityFlip::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo * ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData * TriggerEventData)
{
	// if (!ActorInfo->IsNetAuthority())
	// {
	// 	return;
	// }

	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		}
		
		if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
		{
			if (AAlsxtCharacter* Character = Cast<AAlsxtCharacter>(ActorInfo->AvatarActor.Get()))
			{
				// if (Character->CanJump() && StaminaCostEffect)
				// {
				// 	if (StaminaCostEffect)
				// 	{
				// 		// Create a new effect context
				// 		FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
				// 		EffectContext.AddInstigator(ActorInfo->AvatarActor.Get(), ActorInfo->AvatarActor.Get());
// 
				// 		// Create the effect spec and set the cost
				// 		UGameplayEffect* CostEffect = StaminaCostEffect->GetDefaultObject<UGameplayEffect>();
				// 		EffectContext.AddInstigator(ActorInfo->AvatarActor.Get(), ActorInfo->AvatarActor.Get());
				// 		// FGameplayEffectSpecHandle CostSpecHandle = ASC->MakeOutgoingGameplayEffectSpec(StaminaCostEffect, GetAbilityLevel());
				// 		StaminaDrainEffectSpecHandle = ASC->MakeOutgoingSpec(StaminaCostEffect, GetAbilityLevel(), EffectContext);
				// 		StaminaDrainEffectSpecHandle.Data->SetSetByCallerMagnitude(StaminaCostTag, -BaseJumpStaminaCost); // Use negative for costs
// 
				// 		// Apply the stamina cost effect
				// 		ActorInfo->AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*StaminaDrainEffectSpecHandle.Data.Get());
				// 	}
				// 	
				// 	// Perform the jump action
				// 	Character->Jump();
// 
				// 	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
				// }
				// New
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
				// Perform the jump action
				Character->Jump();

				EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
			}
		}
	}
}

bool UAlsxtGameplayAbilityFlip::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo * ActorInfo, const FGameplayTagContainer * SourceTags, const FGameplayTagContainer * TargetTags, OUT FGameplayTagContainer * OptionalRelevantTags) const
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
	
	const ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get(), ECastCheckedType::NullAllowed);
	float CurrentStamina = StaminaSet->GetCurrentStamina();
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
					return CurrentStamina >= FMath::Abs(JumpCost) && Character->CanJump(); // Use FMath::Abs as cost is likely negative
				}
			}
		}
		return Character->CanJump();
	}
	return Character->CanJump();
}

void UAlsxtGameplayAbilityFlip::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo * ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	if (ScopeLockCount > 0)
	{
		WaitingToExecute.Add(FPostLockDelegate::CreateUObject(this, &UAlsxtGameplayAbilityFlip::CancelAbility, Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility));
		return;
	}
	
	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);

	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	ACharacter * Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
	Character->StopJumping();
}

void UAlsxtGameplayAbilityFlip::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// AAlsxtCharacter* Character = Cast<AAlsxtCharacter>(ActorInfo->AvatarActor.Get());
	// Character->SetDesiredGait(AlsGaitTags::Running);

	// Try to Activate Stamina Regen Ability
	FGameplayTagContainer StaminaRegenGameplayTags;
	StaminaRegenGameplayTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Gameplay.Ability.StaminaRegen")));
	if (ActorInfo->AbilitySystemComponent->TryActivateAbilitiesByTag(StaminaRegenGameplayTags, true))
	{
		UE_LOG(LogTemp, Warning, TEXT("UAlsxtGameplayAbilityFlip::EndAbility: Gameplay.Ability.StaminaRegen activated!"));
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}


