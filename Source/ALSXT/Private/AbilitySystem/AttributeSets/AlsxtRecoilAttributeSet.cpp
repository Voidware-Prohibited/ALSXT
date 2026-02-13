// Copyright (C) 2026 Uriel Ballinas, VOIDWARE Prohibited. All rights reserved.
// This software is licensed under the MIT License (LICENSE.md).



#include "AbilitySystem/AttributeSets/AlsxtRecoilAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

float UAlsxtRecoilAttributeSet::RecoilStrength_BASE = 1.0f; 
float UAlsxtRecoilAttributeSet::RecoilStrength_MAX = 10.0f;
float UAlsxtRecoilAttributeSet::RecoilStrength_MIN = 0.01f;

float UAlsxtRecoilAttributeSet::RecoilClimb_BASE = 1.0f; 
float UAlsxtRecoilAttributeSet::RecoilClimb_MAX = 10.0f;
float UAlsxtRecoilAttributeSet::RecoilClimb_MIN = 0.01f;

float UAlsxtRecoilAttributeSet::RecoilDrift_BASE = 1.0f; 
float UAlsxtRecoilAttributeSet::RecoilDrift_MAX = 10.0f;
float UAlsxtRecoilAttributeSet::RecoilDrift_MIN = 0.01f;

float UAlsxtRecoilAttributeSet::RecoilStability_BASE = 1.0f; 
float UAlsxtRecoilAttributeSet::RecoilStability_MAX = 10.0f;
float UAlsxtRecoilAttributeSet::RecoilStability_MIN = 0.01f;

float UAlsxtRecoilAttributeSet::RecoilKickback_BASE = 1.0f; 
float UAlsxtRecoilAttributeSet::RecoilKickback_MAX = 10.0f;
float UAlsxtRecoilAttributeSet::RecoilKickback_MIN = 0.01f;

float UAlsxtRecoilAttributeSet::RecoilNoiseStrength_BASE = 1.0f; 
float UAlsxtRecoilAttributeSet::RecoilNoiseStrength_MAX = 10.0f;
float UAlsxtRecoilAttributeSet::RecoilNoiseStrength_MIN = 0.01f;


UAlsxtRecoilAttributeSet::UAlsxtRecoilAttributeSet()
{
	RecoilStrength = 1.0f;
	RecoilClimb = 1.0f;
	RecoilDrift = 1.0f;
	RecoilStability = 1.0f;
	RecoilKickback = 1.0f;
	RecoilNoiseStrength = 1.0f;
}

void UAlsxtRecoilAttributeSet::PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const
{
	Super::PostAttributeBaseChange(Attribute, OldValue, NewValue);
}

void UAlsxtRecoilAttributeSet::ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::ClampAttributes(Attribute, NewValue);
	if (Attribute == GetRecoilStrengthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, RecoilStrength_MIN, RecoilStrength_MAX);
		return;
	}
	if (Attribute == GetRecoilClimbAttribute())
	{
		NewValue = FMath::Clamp(NewValue, RecoilClimb_MIN, RecoilClimb_MAX);
		return;
	}
	if (Attribute == GetRecoilDriftAttribute())
	{
		NewValue = FMath::Clamp(NewValue, RecoilDrift_MIN, RecoilDrift_MAX);
		return;
	}
	if (Attribute == GetRecoilStabilityAttribute())
	{
		NewValue = FMath::Clamp(NewValue, RecoilStability_MIN, RecoilStability_MAX);
		return;
	}
	if (Attribute == GetRecoilKickbackAttribute())
	{
		NewValue = FMath::Clamp(NewValue, RecoilKickback_MIN, RecoilKickback_MAX);
		return;
	}
	if (Attribute == GetRecoilNoiseStrengthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, RecoilNoiseStrength_MIN, RecoilNoiseStrength_MAX);
		return;
	}
}

void UAlsxtRecoilAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params{};
	Params.bIsPushBased = true;
	Params.Condition = COND_OwnerOnly;

	// Replicated to all
	DOREPLIFETIME_WITH_PARAMS_FAST(UAlsxtRecoilAttributeSet, RecoilStrength, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAlsxtRecoilAttributeSet, RecoilClimb, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAlsxtRecoilAttributeSet, RecoilDrift, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAlsxtRecoilAttributeSet, RecoilStability, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAlsxtRecoilAttributeSet, RecoilKickback, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAlsxtRecoilAttributeSet, RecoilNoiseStrength, Params);
}

void UAlsxtRecoilAttributeSet::OnRep_RecoilStrength(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlsxtRecoilAttributeSet, RecoilStrength, OldValue);
}

void UAlsxtRecoilAttributeSet::OnRep_RecoilClimb(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlsxtRecoilAttributeSet, RecoilClimb, OldValue);
}

void UAlsxtRecoilAttributeSet::OnRep_RecoilDrift(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlsxtRecoilAttributeSet, RecoilDrift, OldValue);
}

void UAlsxtRecoilAttributeSet::OnRep_RecoilStability(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlsxtRecoilAttributeSet, RecoilStability, OldValue);
}

void UAlsxtRecoilAttributeSet::OnRep_RecoilKickback(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlsxtRecoilAttributeSet, RecoilKickback, OldValue);
}

void UAlsxtRecoilAttributeSet::OnRep_RecoilNoiseStrength(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlsxtRecoilAttributeSet, RecoilNoiseStrength, OldValue);
}
