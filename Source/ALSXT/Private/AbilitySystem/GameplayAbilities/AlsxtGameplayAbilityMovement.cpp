// MIT


#include "AbilitySystem/GameplayAbilities/AlsxtGameplayAbilityMovement.h"

#include "AbilitySystem/AttributeSets/AlsxtMovementAttributeSet.h"

UAlsxtGameplayAbilityMovement::UAlsxtGameplayAbilityMovement()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	FGameplayTagContainer AssetTags;
	AssetTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Gameplay.Ability.Movement")));
	SetAssetTags(AssetTags);
}

void UAlsxtGameplayAbilityMovement::PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate,
	const FGameplayEventData* TriggerEventData)
{
	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);
}

void UAlsxtGameplayAbilityMovement::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                                    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Set Stamina Drain Magnitude (From Stance and Gait)
	// Else Set Stamina Regen Magnitude (From Stance and Gait)

	 if (HasAuthority(&ActivationInfo))
    {
	 	float BaseSpeed = 500.0;

	 	GetAbilitySystemComponentFromActorInfo()->SetNumericAttributeBase(UAlsxtMovementAttributeSet::GetMovementSpeedAttribute(), BaseSpeed);

	 	// 3. Apply Tags so CMC knows to look at ASC.
	 	// The Active Gameplay Effects will automatically multiply/add to this value.
    }
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UAlsxtGameplayAbilityMovement::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                                  const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UAlsxtGameplayAbilityMovement::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
