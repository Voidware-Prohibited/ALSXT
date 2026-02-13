// MIT


#include "AbilitySystem/AttributeSets/AlsxtBodyTempAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/Data/AlsxtGASGameplayTags.h"
#include "Net/UnrealNetwork.h"

UAlsxtBodyTempAttributeSet::UAlsxtBodyTempAttributeSet()
{
	MaximumBodyTemp = 1.0f;
	CurrentBodyTemp = 1.0f;
	BodyTempRegeneration = 0.0f;
	Bleeding = 0.f;
	BleedHealing = 0.f;
}

void UAlsxtBodyTempAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params{};
	Params.bIsPushBased = true;
	Params.Condition = COND_None;

	// Replicated to all
	DOREPLIFETIME_WITH_PARAMS_FAST(UAlsxtBodyTempAttributeSet, CurrentBodyTemp, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAlsxtBodyTempAttributeSet, MaximumBodyTemp, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAlsxtBodyTempAttributeSet, Bleeding, Params);

	// Only Owner
	Params.Condition = COND_OwnerOnly;
	DOREPLIFETIME_WITH_PARAMS_FAST(UAlsxtBodyTempAttributeSet, BodyTempRegeneration, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAlsxtBodyTempAttributeSet, BleedHealing, Params);
}

void UAlsxtBodyTempAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
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

void UAlsxtBodyTempAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// Store a local copy of the amount of Damage done and clear the Damage attribute.
		const float LocalDamageDone = GetDamage();

		SetDamage(0.f);
	
		if (LocalDamageDone > 0.0f)
		{
			// Apply the BodyTemp change and then clamp it.
			const float NewBodyTemp = GetCurrentBodyTemp() - LocalDamageDone;

			SetCurrentBodyTemp(FMath::Clamp(NewBodyTemp, 0.0f, GetMaximumBodyTemp()));
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
			// Apply the BodyTemp change and then clamp it.
			const float NewBodyTemp = GetCurrentBodyTemp() + LocalHealingDone;

			SetCurrentBodyTemp(FMath::Clamp(NewBodyTemp, 0.0f, GetMaximumBodyTemp()));
		}
		return;
	}

	if (Data.EvaluatedData.Attribute == GetBodyTempRegenerationAttribute())
	{
		SetBodyTempRegeneration(FMath::Clamp(GetBodyTempRegeneration(), 0.0f, GetMaximumBodyTemp()));
		return;
	}
}

void UAlsxtBodyTempAttributeSet::OnRep_CurrentBodyTemp(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlsxtBodyTempAttributeSet, CurrentBodyTemp, OldValue);
}

void UAlsxtBodyTempAttributeSet::OnRep_MaximumBodyTemp(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlsxtBodyTempAttributeSet, MaximumBodyTemp, OldValue);
}

void UAlsxtBodyTempAttributeSet::OnRep_BodyTempRegeneration(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlsxtBodyTempAttributeSet, BodyTempRegeneration, OldValue);
}

void UAlsxtBodyTempAttributeSet::OnRep_Bleeding(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlsxtBodyTempAttributeSet, Bleeding, OldValue);
}

void UAlsxtBodyTempAttributeSet::OnRep_BleedHealing(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlsxtBodyTempAttributeSet, BleedHealing, OldValue);
}

void UAlsxtBodyTempAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	
	if (Attribute == GetCurrentBodyTempAttribute())
	{
		CheckMaxReachedForAttribute(MaximumBodyTemp, AlsxtGASGameplayTags::State::TAG_State_Max_BodyTemp.GetTag(), NewValue);
		return;
	}
	
	if (Attribute == GetBleedingAttribute())
	{
		CheckStatusTagForAttribute(AlsxtGASGameplayTags::Statuses::TAG_Debuff_Bleeding, NewValue, OldValue);
		return;
	}

	if (Attribute == GetMaximumBodyTempAttribute())
	{
		AdjustAttributeForMaxChange(GetCurrentBodyTempAttribute(), OldValue, NewValue);
		return;
	}
}

void UAlsxtBodyTempAttributeSet::PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const
{
	Super::PostAttributeBaseChange(Attribute, OldValue, NewValue);
}

void UAlsxtBodyTempAttributeSet::ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetMaximumBodyTempAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.f);
		return;
	}
	if (Attribute == GetCurrentBodyTempAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaximumBodyTemp());
		return;
	}
	if (Attribute == GetBleedingAttribute() || Attribute == GetBleedHealingAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.f);
		return;
	}
}


