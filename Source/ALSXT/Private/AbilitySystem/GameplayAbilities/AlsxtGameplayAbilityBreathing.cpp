#include "AbilitySystem/GameplayAbilities/AlsxtGameplayAbilityBreathing.h"
#include "AbilitySystemComponent.h"
#include "ChooserFunctionLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameplayEffect.h"
#include "AbilitySystem/AttributeSets/AlsxtBreathAttributeSet.h"
#include "GameFramework/Character.h"
#include "AbilitySystem/AttributeSets/AlsxtStaminaAttributeSet.h"

UAlsxtGameplayAbilityBreathing::UAlsxtGameplayAbilityBreathing()
{
	// Set the ability to be passive and auto-activate.
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Gameplay.State.Dead")));
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Gameplay.Ability.Breathing")));
	// bServerRespectsRemoteActivation = false;
	bServerRespectsRemoteAbilityCancellation = false;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;
}

void UAlsxtGameplayAbilityBreathing::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		if (ActorInfo->IsNetAuthority())
		{
			FGameplayEffectContextHandle EffectContext = ActorInfo->AbilitySystemComponent->MakeEffectContext();
			EffectContext.AddSourceObject(this);

			// FGameplayEffectSpecHandle SpecHandle = ActorInfo->AbilitySystemComponent->MakeOutgoingGameplayEffectSpec(BreathingEffect, GetAbilityLevel(), EffectContext);
			FGameplayEffectSpecHandle SpecHandle = ActorInfo->AbilitySystemComponent->MakeOutgoingSpec(BreathingEffect, GetAbilityLevel(), EffectContext);

			if (SpecHandle.IsValid())
			{
				ActiveEffectHandle = ActorInfo->AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}

			if (GetAbilitySystemComponentFromActorInfo()) {
				StaminaChangedDelegateHandle = GetAbilitySystemComponentFromActorInfo()->GetGameplayAttributeValueChangeDelegate(UAlsxtStaminaAttributeSet::GetCurrentStaminaAttribute()).AddUObject(this, &UAlsxtGameplayAbilityBreathing::OnStaminaChanged);
			}

			// 2. Play Looping MetaSound using Ability Task
			ActiveBreathComponent = UGameplayStatics::SpawnSoundAttached(BreathingMetaSound, Cast<ACharacter>(GetAvatarActorFromActorInfo())->GetMesh(), VoiceSocketName);
    
			// 3. Setup Deactivation Listeners (Underwater/Dead/Holding Breath)
			// GetAbilitySystemComponentFromActorInfo()->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag("State.Dead"), EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UAlsxtGameplayAbilityBreathing::OnDeactivationTagChanged, FOnAttributeChangeData);
			// ... repeat for Underwater/HoldBreath
		}
	}
}

void UAlsxtGameplayAbilityBreathing::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (ActiveEffectHandle.IsValid())
	{
		ActorInfo->AbilitySystemComponent->RemoveActiveGameplayEffect(ActiveEffectHandle);
	}
	
	if (ActiveBreathComponent)
	{
		ActiveBreathComponent->Stop();
		ActiveBreathComponent->DestroyComponent();
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UAlsxtGameplayAbilityBreathing::OnStaminaChanged(const FOnAttributeChangeData& Data)
{
	RefreshBreathingSounds(Data.NewValue);
}

void UAlsxtGameplayAbilityBreathing::RefreshBreathingSounds(float CurrentStamina)
{
	// 1. Use Chooser Table to determine state (0-100 stamina -> 0-1 breathIntensity)
	// TODO: Fix EvaluateChooser call
	// FInstancedStruct Result = UChooserFunctionLibrary::EvaluateChooser(this, BreathingChooser, FContextData(CurrentStamina));
	// float BreathingIntensity = Result.Get<float>();
	float BreathingIntensity = 0.3;

	// 2. Pass Intensity to MetaSound
	if (ActiveBreathComponent) {
		ActiveBreathComponent->SetFloatParameter(FName("BreathIntensity"), BreathingIntensity);
	}
}
