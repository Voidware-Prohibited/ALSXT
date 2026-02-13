// MIT


#include "AbilitySystem/AttributeSets/AlsxtStrengthAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/Data/AlsxtGASGameplayTags.h"
#include "Net/UnrealNetwork.h"

UAlsxtStrengthAttributeSet::UAlsxtStrengthAttributeSet()
{
	MaximumStrength = 1.0f;
	CurrentStrength = 1.0f;
	StrengthRegeneration = 0.0f;
	Bleeding = 0.f;
	BleedHealing = 0.f;
}

void UAlsxtStrengthAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params{};
	Params.bIsPushBased = true;
	Params.Condition = COND_None;

	// Replicated to all
	DOREPLIFETIME_WITH_PARAMS_FAST(UAlsxtStrengthAttributeSet, CurrentStrength, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAlsxtStrengthAttributeSet, MaximumStrength, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAlsxtStrengthAttributeSet, Bleeding, Params);

	// Only Owner
	Params.Condition = COND_OwnerOnly;
	DOREPLIFETIME_WITH_PARAMS_FAST(UAlsxtStrengthAttributeSet, StrengthRegeneration, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAlsxtStrengthAttributeSet, BleedHealing, Params);
}

void UAlsxtStrengthAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
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

void UAlsxtStrengthAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// Store a local copy of the amount of Damage done and clear the Damage attribute.
		const float LocalDamageDone = GetDamage();

		SetDamage(0.f);
	
		if (LocalDamageDone > 0.0f)
		{
			// Apply the Strength change and then clamp it.
			const float NewStrength = GetCurrentStrength() - LocalDamageDone;

			SetCurrentStrength(FMath::Clamp(NewStrength, 0.0f, GetMaximumStrength()));
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
			// Apply the Strength change and then clamp it.
			const float NewStrength = GetCurrentStrength() + LocalHealingDone;

			SetCurrentStrength(FMath::Clamp(NewStrength, 0.0f, GetMaximumStrength()));
		}
		return;
	}

	if (Data.EvaluatedData.Attribute == GetStrengthRegenerationAttribute())
	{
		SetStrengthRegeneration(FMath::Clamp(GetStrengthRegeneration(), 0.0f, GetMaximumStrength()));
		return;
	}
}

void UAlsxtStrengthAttributeSet::OnRep_CurrentStrength(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlsxtStrengthAttributeSet, CurrentStrength, OldValue);
}

void UAlsxtStrengthAttributeSet::OnRep_MaximumStrength(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlsxtStrengthAttributeSet, MaximumStrength, OldValue);
}

void UAlsxtStrengthAttributeSet::OnRep_StrengthRegeneration(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlsxtStrengthAttributeSet, StrengthRegeneration, OldValue);
}

void UAlsxtStrengthAttributeSet::OnRep_Bleeding(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlsxtStrengthAttributeSet, Bleeding, OldValue);
}

void UAlsxtStrengthAttributeSet::OnRep_BleedHealing(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlsxtStrengthAttributeSet, BleedHealing, OldValue);
}

void UAlsxtStrengthAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	
	if (Attribute == GetCurrentStrengthAttribute())
	{
		CheckMaxReachedForAttribute(MaximumStrength, AlsxtGASGameplayTags::State::TAG_State_Max_Strength.GetTag(), NewValue);
		return;
	}
	
	if (Attribute == GetBleedingAttribute())
	{
		CheckStatusTagForAttribute(AlsxtGASGameplayTags::Statuses::TAG_Debuff_Bleeding, NewValue, OldValue);
		return;
	}

	if (Attribute == GetMaximumStrengthAttribute())
	{
		AdjustAttributeForMaxChange(GetCurrentStrengthAttribute(), OldValue, NewValue);
		return;
	}
}

void UAlsxtStrengthAttributeSet::PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const
{
	Super::PostAttributeBaseChange(Attribute, OldValue, NewValue);
}

void UAlsxtStrengthAttributeSet::ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetMaximumStrengthAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.f);
		return;
	}
	if (Attribute == GetCurrentStrengthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaximumStrength());
		return;
	}
	if (Attribute == GetBleedingAttribute() || Attribute == GetBleedHealingAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.f);
		return;
	}
}


