// MIT


#include "AbilitySystem/AttributeSets/AlsxtNutritionAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/Data/AlsxtGASGameplayTags.h"
#include "Net/UnrealNetwork.h"

UAlsxtNutritionAttributeSet::UAlsxtNutritionAttributeSet()
{
	MaximumNutrition = 1.0f;
	CurrentNutrition = 1.0f;
	NutritionRegeneration = 0.0f;
	Bleeding = 0.f;
	BleedHealing = 0.f;
}

void UAlsxtNutritionAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params{};
	Params.bIsPushBased = true;
	Params.Condition = COND_None;

	// Replicated to all
	DOREPLIFETIME_WITH_PARAMS_FAST(UAlsxtNutritionAttributeSet, CurrentNutrition, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAlsxtNutritionAttributeSet, MaximumNutrition, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAlsxtNutritionAttributeSet, Bleeding, Params);

	// Only Owner
	Params.Condition = COND_OwnerOnly;
	DOREPLIFETIME_WITH_PARAMS_FAST(UAlsxtNutritionAttributeSet, NutritionRegeneration, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAlsxtNutritionAttributeSet, BleedHealing, Params);
}

void UAlsxtNutritionAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
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

void UAlsxtNutritionAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// Store a local copy of the amount of Damage done and clear the Damage attribute.
		const float LocalDamageDone = GetDamage();

		SetDamage(0.f);
	
		if (LocalDamageDone > 0.0f)
		{
			// Apply the Nutrition change and then clamp it.
			const float NewNutrition = GetCurrentNutrition() - LocalDamageDone;

			SetCurrentNutrition(FMath::Clamp(NewNutrition, 0.0f, GetMaximumNutrition()));
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
			// Apply the Nutrition change and then clamp it.
			const float NewNutrition = GetCurrentNutrition() + LocalHealingDone;

			SetCurrentNutrition(FMath::Clamp(NewNutrition, 0.0f, GetMaximumNutrition()));
		}
		return;
	}

	if (Data.EvaluatedData.Attribute == GetNutritionRegenerationAttribute())
	{
		SetNutritionRegeneration(FMath::Clamp(GetNutritionRegeneration(), 0.0f, GetMaximumNutrition()));
		return;
	}
}

void UAlsxtNutritionAttributeSet::OnRep_CurrentNutrition(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlsxtNutritionAttributeSet, CurrentNutrition, OldValue);
}

void UAlsxtNutritionAttributeSet::OnRep_MaximumNutrition(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlsxtNutritionAttributeSet, MaximumNutrition, OldValue);
}

void UAlsxtNutritionAttributeSet::OnRep_NutritionRegeneration(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlsxtNutritionAttributeSet, NutritionRegeneration, OldValue);
}

void UAlsxtNutritionAttributeSet::OnRep_Bleeding(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlsxtNutritionAttributeSet, Bleeding, OldValue);
}

void UAlsxtNutritionAttributeSet::OnRep_BleedHealing(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlsxtNutritionAttributeSet, BleedHealing, OldValue);
}

void UAlsxtNutritionAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	
	if (Attribute == GetCurrentNutritionAttribute())
	{
		CheckMaxReachedForAttribute(MaximumNutrition, AlsxtGASGameplayTags::State::TAG_State_Max_Nutrition.GetTag(), NewValue);
		return;
	}
	
	if (Attribute == GetBleedingAttribute())
	{
		CheckStatusTagForAttribute(AlsxtGASGameplayTags::Statuses::TAG_Debuff_Bleeding, NewValue, OldValue);
		return;
	}

	if (Attribute == GetMaximumNutritionAttribute())
	{
		AdjustAttributeForMaxChange(GetCurrentNutritionAttribute(), OldValue, NewValue);
		return;
	}
}

void UAlsxtNutritionAttributeSet::PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const
{
	Super::PostAttributeBaseChange(Attribute, OldValue, NewValue);
}

void UAlsxtNutritionAttributeSet::ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetMaximumNutritionAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.f);
		return;
	}
	if (Attribute == GetCurrentNutritionAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaximumNutrition());
		return;
	}
	if (Attribute == GetBleedingAttribute() || Attribute == GetBleedHealingAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.f);
		return;
	}
}


