// MIT


#include "AbilitySystem/AttributeSets/AlsxtHoldBreathAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/Data/AlsxtGASGameplayTags.h"
#include "Net/UnrealNetwork.h"

UAlsxtHoldBreathAttributeSet::UAlsxtHoldBreathAttributeSet()
{
	MaxHoldBreathDuration = 1.0f;
	CurrentHoldBreathDuration = 1.0f;
	CurrentHoldBreathDurationRegen = 0.0f;
	MaxHoldBreathDurationRegen = 0.0f;
}

void UAlsxtHoldBreathAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params{};
	Params.bIsPushBased = true;
	Params.Condition = COND_None;

	// Replicated to all
	DOREPLIFETIME_WITH_PARAMS_FAST(UAlsxtHoldBreathAttributeSet, CurrentHoldBreathDuration, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAlsxtHoldBreathAttributeSet, MaxHoldBreathDuration, Params);

	// Only Owner
	Params.Condition = COND_OwnerOnly;
	DOREPLIFETIME_WITH_PARAMS_FAST(UAlsxtHoldBreathAttributeSet, CurrentHoldBreathDurationRegen, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAlsxtHoldBreathAttributeSet, MaxHoldBreathDurationRegen, Params);
}

void UAlsxtHoldBreathAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
	// const FCustomAttributeMaxValue* MaxValue;

	if (const FAlsxtCustomAttributeMaxValue* MaxValue = AttributeMaxValue.Find(Attribute))
	{
		const float Max = MaxValue->MaxAttribute.IsValid() ? MaxValue->MaxAttribute.GetNumericValue(this) : MaxValue->MaxFloat.GetValueAtLevel(0);

		NewValue = FMath::Clamp(NewValue, 0.f, Max);
		return;
	}
}

void UAlsxtHoldBreathAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetCurrentHoldBreathDurationAttribute())
	{
		// Store a local copy CurrentHoldBreathDuration and clear the CurrentHoldBreathDuration attribute.
		const float LocalCurrentHoldBreathDuration = GetCurrentHoldBreathDuration();

		SetCurrentHoldBreathDuration(0.f);
	
		if (LocalCurrentHoldBreathDuration > 0.0f)
		{
			// Apply the CurrentHoldBreathDuration change and then clamp it.
			const float NewCurrentHoldBreathDuration = GetCurrentHoldBreathDuration() - LocalCurrentHoldBreathDuration;

			SetCurrentHoldBreathDuration(FMath::Clamp(NewCurrentHoldBreathDuration, 0.0f, GetMaxHoldBreathDuration()));
		}
		return;
	}
	

	if (Data.EvaluatedData.Attribute == GetCurrentHoldBreathDurationRegenAttribute())
	{
		SetCurrentHoldBreathDurationRegen(FMath::Clamp(GetCurrentHoldBreathDurationRegen(), 0.0f, GetMaxHoldBreathDuration()));
		return;
	}
}

void UAlsxtHoldBreathAttributeSet::OnRep_CurrentHoldBreathDuration(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlsxtHoldBreathAttributeSet, CurrentHoldBreathDuration, OldValue);
}

void UAlsxtHoldBreathAttributeSet::OnRep_MaxHoldBreathDuration(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlsxtHoldBreathAttributeSet, MaxHoldBreathDuration, OldValue);
}

void UAlsxtHoldBreathAttributeSet::OnRep_CurrentHoldBreathDurationRegen(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlsxtHoldBreathAttributeSet, CurrentHoldBreathDurationRegen, OldValue);
}

void UAlsxtHoldBreathAttributeSet::OnRep_MaxHoldBreathDurationRegen(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlsxtHoldBreathAttributeSet, MaxHoldBreathDurationRegen, OldValue);
}

void UAlsxtHoldBreathAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	
	if (Attribute == GetCurrentHoldBreathDurationAttribute())
	{
		CheckMaxReachedForAttribute(MaxHoldBreathDuration, AlsxtGASGameplayTags::State::TAG_State_Max_HoldingBreath.GetTag(), NewValue);
		return;
	}

	if (Attribute == GetCurrentHoldBreathDurationRegenAttribute())
	{
		CheckMaxReachedForAttribute(MaxHoldBreathDurationRegen, AlsxtGASGameplayTags::State::TAG_State_Max_HoldingBreathRegen.GetTag(), NewValue);
		return;
	}

	if (Attribute == GetMaxHoldBreathDurationAttribute())
	{
		AdjustAttributeForMaxChange(GetCurrentHoldBreathDurationAttribute(), OldValue, NewValue);
		return;
	}

	if (Attribute == GetMaxHoldBreathDurationRegenAttribute())
	{
		AdjustAttributeForMaxChange(GetCurrentHoldBreathDurationRegenAttribute(), OldValue, NewValue);
		return;
	}
}

void UAlsxtHoldBreathAttributeSet::PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const
{
	Super::PostAttributeBaseChange(Attribute, OldValue, NewValue);
}

void UAlsxtHoldBreathAttributeSet::ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetMaxHoldBreathDurationAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.f);
		return;
	}
	if (Attribute == GetMaxHoldBreathDurationRegenAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.f);
		return;
	}
	if (Attribute == GetCurrentHoldBreathDurationAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHoldBreathDuration());
		return;
	}
	if (Attribute == GetCurrentHoldBreathDurationRegenAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHoldBreathDurationRegen());
		return;
	}

}


