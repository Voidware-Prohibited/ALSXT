// MIT

#include "AbilitySystem/AttributeSets/AlsxtMovementAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UAlsxtMovementAttributeSet::UAlsxtMovementAttributeSet()
{
	MovementSpeedMultiplier = 1.0f;
	MovementAccelerationMultiplier = 1.0f;
}

void UAlsxtMovementAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	// Make sure we don't change the base at this is always supposed to return a multiplier. 
	NewValue = 1.f;
}

void UAlsxtMovementAttributeSet::ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::ClampAttributes(Attribute, NewValue);

	// Make sure 
	NewValue = FMath::Max(NewValue, 0.f);
}

void UAlsxtMovementAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params{};
	Params.bIsPushBased = true;
	Params.Condition = COND_OwnerOnly;

	// Replicated to all
	DOREPLIFETIME_WITH_PARAMS_FAST(UAlsxtMovementAttributeSet, MovementSpeed, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAlsxtMovementAttributeSet, MovementSpeedMultiplier, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAlsxtMovementAttributeSet, MovementAccelerationMultiplier, Params);
}

void UAlsxtMovementAttributeSet::OnRep_MovementSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlsxtMovementAttributeSet, MovementSpeed, OldValue);
}

void UAlsxtMovementAttributeSet::OnRep_MovementSpeedMultiplier(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlsxtMovementAttributeSet, MovementSpeedMultiplier, OldValue);
}

void UAlsxtMovementAttributeSet::OnRep_MovementAccelerationMultiplier(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlsxtMovementAttributeSet, MovementAccelerationMultiplier, OldValue);
}