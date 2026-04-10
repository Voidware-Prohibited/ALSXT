// MIT


#include "AbilitySystem/GameplayAbilities/AlsxtGameplayAbilityReadiness.h"
#include "AbilitySystem/AbilityTasks/AlsxtAbilityTaskWaitEnhancedInputEvent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AlsxtCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"

class UAlsxtAbilityTaskWaitEnhancedInputEvent;

UAlsxtGameplayAbilityReadiness::UAlsxtGameplayAbilityReadiness()
{
	// Initialize tags in constructor
	ReadyTag = FGameplayTag::RequestGameplayTag(FName("Als.Combat Stance.Ready"));
	AimingTag = FGameplayTag::RequestGameplayTag(FName("Als.Combat Stance.Aiming"));
	NeutralTag = FGameplayTag::RequestGameplayTag(FName("Als.Combat Stance.Neutral"));

	FGameplayTagContainer AssetTags = { };
	AssetTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Gameplay.Ability.Readiness")));
	SetAssetTags(AssetTags);
}

bool UAlsxtGameplayAbilityReadiness::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	//ACustomPlayerState* PS = Cast<ACustomPlayerState>(ActorInfo->PlayerState.Get());
	//return PS && PS->CanBecomeCombatReady();
	return true;
}

void UAlsxtGameplayAbilityReadiness::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!HasAuthority(&ActivationInfo)) return;

	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
			return;
		}

		// Get Player State and check for combat readiness
		// ACustomPlayerState* PlayerState = Cast<ACustomPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
		// if (!PlayerState || !PlayerState->CanBecomeCombatReady())
		// {
		// 	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		// 	return;
		// }
        
		// Add the Ready tag initially
		ApplyTagGameplayEffect(ReadyGameplayEffect);

		if (GetAvatarActorFromActorInfo()->Implements<UAlsxtCharacterInterface>())
		{
			if (IAlsxtCharacterInterface::Execute_GetCharacterReadiness(GetAvatarActorFromActorInfo()) == AlsxtReadinessTags::Relaxed || IAlsxtCharacterInterface::Execute_GetCharacterReadiness(GetAvatarActorFromActorInfo()) == AlsxtReadinessTags::Carry)
			{
				IAlsxtCharacterInterface::Execute_SetCharacterReadiness(GetAvatarActorFromActorInfo(), AlsxtReadinessTags::Ready);
			}
			if (IAlsxtCharacterInterface::Execute_GetCharacterReadiness(GetAvatarActorFromActorInfo()) == AlsxtReadinessTags::Ready || IAlsxtCharacterInterface::Execute_GetCharacterReadiness(GetAvatarActorFromActorInfo()) == AlsxtReadinessTags::Aiming)
			{
				IAlsxtCharacterInterface::Execute_SetCharacterReadiness(GetAvatarActorFromActorInfo(), AlsxtReadinessTags::Carry);
			}
		}

		// Wait for Input Press
		// UAbilityTask_WaitInputRelease* WaitInputReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this);
		// if (WaitInputReleaseTask)
		// {
		// 	ApplyTagGameplayEffect(AimingGameplayEffect);
		// 	// Bind a function to the OnRelease delegate
		// 	WaitInputReleaseTask->OnRelease.AddDynamic(this, &UAlsxtGameplayAbilityReadiness::OnInputReleased);
		// 	// Activate the task
		// 	WaitInputReleaseTask->ReadyForActivation();
		// }

		

	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UAlsxtGameplayAbilityReadiness::OnInputReleased(float TimeHeld)
{
	// Logic to execute when input is released
	UE_LOG(LogTemp, Warning, TEXT("Input Released! Time held: %f"), TimeHeld);

	// if (GetAvatarActorFromActorInfo()->Implements<UAlsxtCharacterInterface>())
	// {
	// 	if (IAlsxtCharacterInterface::Execute_GetCharacterReadiness(GetAvatarActorFromActorInfo()) == AlsxtReadinessTags::Relaxed || IAlsxtCharacterInterface::Execute_GetCharacterReadiness(GetAvatarActorFromActorInfo()) == AlsxtReadinessTags::Carry)
	// 	{
	// 		IAlsxtCharacterInterface::Execute_SetCharacterReadiness(GetAvatarActorFromActorInfo(), AlsxtReadinessTags::Ready);
	// 	}
	// 	if (IAlsxtCharacterInterface::Execute_GetCharacterReadiness(GetAvatarActorFromActorInfo()) == AlsxtReadinessTags::Ready || IAlsxtCharacterInterface::Execute_GetCharacterReadiness(GetAvatarActorFromActorInfo()) == AlsxtReadinessTags::Aiming)
	// 	{
	// 		IAlsxtCharacterInterface::Execute_SetCharacterReadiness(GetAvatarActorFromActorInfo(), AlsxtReadinessTags::Carry);
	// 	}
	// }
	
	// ApplyTagGameplayEffect(ReadyGameplayEffect);
}

void UAlsxtGameplayAbilityReadiness::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (ScopeLockCount > 0)
	{
		WaitingToExecute.Add(FPostLockDelegate::CreateUObject(this, &UAlsxtGameplayAbilityReadiness::EndAbility, Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled));
		return;
	}

	// Remove old tags and apply the replacement tag
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
	if (ASC)
	{
		ASC->RemoveLooseGameplayTag(ReadyTag);
		ASC->RemoveLooseGameplayTag(AimingTag);
		ApplyTagGameplayEffect(FinalReplacementGameplayEffect);
	}
    
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UAlsxtGameplayAbilityReadiness::ApplyTagGameplayEffect(TSubclassOf<UGameplayEffect> EffectClass)
{
    UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
    if (ASC && EffectClass)
    {
        FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
        FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(EffectClass, 1, ContextHandle);
    	
        if (SpecHandle.IsValid())
        {
            ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
        }
    }
}