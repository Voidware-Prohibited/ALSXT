// MIT


#include "AbilitySystem/GameplayAbilities/AlsxtGameplayAbilityBlock.h"

#include "AlsxtCharacter.h"


UAlsxtGameplayAbilityBlock::UAlsxtGameplayAbilityBlock()
{
	FGameplayTagContainer AssetTags = { };
	AssetTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Gameplay.Ability.Block")));
	SetAssetTags(AssetTags);
}

void UAlsxtGameplayAbilityBlock::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                                 const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (AAlsxtCharacter* Character = Cast<AAlsxtCharacter>(ActorInfo->AvatarActor.Get()))
	{
		FAlsxtDefensiveModeState PreviousDefensiveModeState = Character->GetDefensiveModeState();
		FAlsxtDefensiveModeState NewDefensiveModeState = Character->GetDefensiveModeState();
		NewDefensiveModeState.Mode = ALSXTDefensiveModeTags::Anticipation;
		NewDefensiveModeState.AnticipationMode = ALSXTDefensiveModeTags::Anticipation;
		NewDefensiveModeState.AnticipationSide = PreviousDefensiveModeState.AnticipationSide == FGameplayTag::EmptyTag ? ALSXTImpactSideTags::Front : PreviousDefensiveModeState.AnticipationSide;
		NewDefensiveModeState.AnticipationHeight = PreviousDefensiveModeState.AnticipationHeight == FGameplayTag::EmptyTag ? ALSXTImpactHeightTags::Middle : PreviousDefensiveModeState.AnticipationHeight;
		NewDefensiveModeState.ObstacleMode = FGameplayTag::EmptyTag;
		NewDefensiveModeState.ObstacleSide = FGameplayTag::EmptyTag;
		NewDefensiveModeState.ObstacleHeight = FGameplayTag::EmptyTag;
		Character->SetDefensiveModeState(NewDefensiveModeState);
	}
}

void UAlsxtGameplayAbilityBlock::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
}

void UAlsxtGameplayAbilityBlock::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (AAlsxtCharacter* Character = Cast<AAlsxtCharacter>(ActorInfo->AvatarActor.Get()))
	{
		FAlsxtDefensiveModeState PreviousDefensiveModeState = Character->GetDefensiveModeState();
		FAlsxtDefensiveModeState NewDefensiveModeState = Character->GetDefensiveModeState();
		NewDefensiveModeState.Mode = PreviousDefensiveModeState.Mode == ALSXTDefensiveModeTags::Anticipation ? FGameplayTag::EmptyTag : PreviousDefensiveModeState.Mode;
		NewDefensiveModeState.AnticipationMode = FGameplayTag::EmptyTag;
		NewDefensiveModeState.AnticipationSide = FGameplayTag::EmptyTag;
		NewDefensiveModeState.AnticipationHeight = FGameplayTag::EmptyTag;
		NewDefensiveModeState.ObstacleMode = FGameplayTag::EmptyTag;
		NewDefensiveModeState.ObstacleSide = FGameplayTag::EmptyTag;
		NewDefensiveModeState.ObstacleHeight = FGameplayTag::EmptyTag;
		Character->SetDefensiveModeState(NewDefensiveModeState);
	}
}

bool UAlsxtGameplayAbilityBlock::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (AAlsxtCharacter* Character = Cast<AAlsxtCharacter>(ActorInfo->AvatarActor.Get()))
	{
		return Character->CanEnterBlockingDefensiveMode();
	}
	return false;
}
