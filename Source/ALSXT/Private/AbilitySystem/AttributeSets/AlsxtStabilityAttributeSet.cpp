// MIT


#include "AbilitySystem/AttributeSets/AlsxtStabilityAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/Data/AlsxtGASGameplayTags.h"
#include "Net/UnrealNetwork.h"

UAlsxtStabilityAttributeSet::UAlsxtStabilityAttributeSet()
{
	MaximumStability = 1.0f;
	CurrentStability = 1.0f;
	StabilityRegeneration = 0.0f;
	Bleeding = 0.f;
	BleedHealing = 0.f;
}

void UAlsxtStabilityAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params{};
	Params.bIsPushBased = true;
	Params.Condition = COND_None;

	// Replicated to all
	DOREPLIFETIME_WITH_PARAMS_FAST(UAlsxtStabilityAttributeSet, CurrentStability, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAlsxtStabilityAttributeSet, MaximumStability, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAlsxtStabilityAttributeSet, Bleeding, Params);

	// Only Owner
	Params.Condition = COND_OwnerOnly;
	DOREPLIFETIME_WITH_PARAMS_FAST(UAlsxtStabilityAttributeSet, StabilityRegeneration, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAlsxtStabilityAttributeSet, BleedHealing, Params);
}

void UAlsxtStabilityAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
	// const FCustomAttributeMaxValue* MaximumValue;

	if (const FAlsxtCustomAttributeMaxValue* MaximumValue = AttributeMaxValue.Find(Attribute))
	{
		// const float Max = MaximumValue->MaxAttribute.IsValid() ? MaximumValue->MaxAttribute.GetNumericValue(this) : MaximumValue->MaxFloat.GetValueAtLevel(0);

		// NewValue = FMath::Clamp(NewValue, 0.f, Max);
		return;
	}
}

void UAlsxtStabilityAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// Store a local copy of the amount of Damage done and clear the Damage attribute.
		const float LocalDamageDone = GetDamage();

		SetDamage(0.f);
	
		if (LocalDamageDone > 0.0f)
		{
			// Apply the Stability change and then clamp it.
			const float NewStability = GetCurrentStability() - LocalDamageDone;

			SetCurrentStability(FMath::Clamp(NewStability, 0.0f, GetMaximumStability()));
		}
		return;
	}

	if (Data.EvaluatedData.Attribute == GetHealingAttribute())
	{
		// Store a local copy of the amount of Healing done and clear the Healing attribute.
		const float LocalHealingDone = GetHealing();

		SetHealing(0.f);
	
		if (LocalHealingDone > 0.0f)
		{
			// Apply the Stability change and then clamp it.
			const float NewStability = GetCurrentStability() + LocalHealingDone;

			SetCurrentStability(FMath::Clamp(NewStability, 0.0f, GetMaximumStability()));
		}
		return;
	}

	if (Data.EvaluatedData.Attribute == GetStabilityRegenerationAttribute())
	{
		SetStabilityRegeneration(FMath::Clamp(GetStabilityRegeneration(), 0.0f, GetMaximumStability()));
		return;
	}
}

void UAlsxtStabilityAttributeSet::OnRep_CurrentStability(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlsxtStabilityAttributeSet, CurrentStability, OldValue);
}

void UAlsxtStabilityAttributeSet::OnRep_MaximumStability(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlsxtStabilityAttributeSet, MaximumStability, OldValue);
}

void UAlsxtStabilityAttributeSet::OnRep_StabilityRegeneration(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlsxtStabilityAttributeSet, StabilityRegeneration, OldValue);
}

void UAlsxtStabilityAttributeSet::OnRep_Bleeding(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlsxtStabilityAttributeSet, Bleeding, OldValue);
}

void UAlsxtStabilityAttributeSet::OnRep_BleedHealing(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlsxtStabilityAttributeSet, BleedHealing, OldValue);
}

void UAlsxtStabilityAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	
	if (Attribute == GetCurrentStabilityAttribute())
	{
		CheckMaxReachedForAttribute(MaximumStability, AlsxtGASGameplayTags::State::TAG_State_Max_Stability.GetTag(), NewValue);
		return;
	}
	
	if (Attribute == GetBleedingAttribute())
	{
		CheckStatusTagForAttribute(AlsxtGASGameplayTags::Statuses::TAG_Debuff_Bleeding, NewValue, OldValue);
		return;
	}

	if (Attribute == GetMaximumStabilityAttribute())
	{
		AdjustAttributeForMaxChange(GetCurrentStabilityAttribute(), OldValue, NewValue);
		return;
	}
}

void UAlsxtStabilityAttributeSet::PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const
{
	Super::PostAttributeBaseChange(Attribute, OldValue, NewValue);
}

void UAlsxtStabilityAttributeSet::ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetMaximumStabilityAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.f);
		return;
	}
	if (Attribute == GetCurrentStabilityAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaximumStability());
		return;
	}
	if (Attribute == GetBleedingAttribute() || Attribute == GetBleedHealingAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.f);
		return;
	}
}


