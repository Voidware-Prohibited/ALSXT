// MIT


#include "AbilitySystem/AttributeSets/AlsxtBloodAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/Data/AlsxtGASGameplayTags.h"
#include "Net/UnrealNetwork.h"

UAlsxtBloodAttributeSet::UAlsxtBloodAttributeSet()
{
	MaximumBlood = 1.0f;
	CurrentBlood = 1.0f;
	BloodRegeneration = 0.0f;
	Bleeding = 0.f;
	BleedHealing = 0.f;
}

void UAlsxtBloodAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params{};
	Params.bIsPushBased = true;
	Params.Condition = COND_None;

	// Replicated to all
	DOREPLIFETIME_WITH_PARAMS_FAST(UAlsxtBloodAttributeSet, CurrentBlood, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAlsxtBloodAttributeSet, MaximumBlood, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAlsxtBloodAttributeSet, Bleeding, Params);

	// Only Owner
	Params.Condition = COND_OwnerOnly;
	DOREPLIFETIME_WITH_PARAMS_FAST(UAlsxtBloodAttributeSet, BloodRegeneration, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAlsxtBloodAttributeSet, BleedHealing, Params);
}

void UAlsxtBloodAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
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

void UAlsxtBloodAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// Store a local copy of the amount of Damage done and clear the Damage attribute.
		const float LocalDamageDone = GetDamage();

		SetDamage(0.f);
	
		if (LocalDamageDone > 0.0f)
		{
			// Apply the Blood change and then clamp it.
			const float NewBlood = GetCurrentBlood() - LocalDamageDone;

			SetCurrentBlood(FMath::Clamp(NewBlood, 0.0f, GetMaximumBlood()));
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
			// Apply the Blood change and then clamp it.
			const float NewBlood = GetCurrentBlood() + LocalHealingDone;

			SetCurrentBlood(FMath::Clamp(NewBlood, 0.0f, GetMaximumBlood()));
		}
		return;
	}

	if (Data.EvaluatedData.Attribute == GetBloodRegenerationAttribute())
	{
		SetBloodRegeneration(FMath::Clamp(GetBloodRegeneration(), 0.0f, GetMaximumBlood()));
		return;
	}
}

void UAlsxtBloodAttributeSet::OnRep_CurrentBlood(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlsxtBloodAttributeSet, CurrentBlood, OldValue);
}

void UAlsxtBloodAttributeSet::OnRep_MaximumBlood(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlsxtBloodAttributeSet, MaximumBlood, OldValue);
}

void UAlsxtBloodAttributeSet::OnRep_BloodRegeneration(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlsxtBloodAttributeSet, BloodRegeneration, OldValue);
}

void UAlsxtBloodAttributeSet::OnRep_Bleeding(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlsxtBloodAttributeSet, Bleeding, OldValue);
}

void UAlsxtBloodAttributeSet::OnRep_BleedHealing(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlsxtBloodAttributeSet, BleedHealing, OldValue);
}

void UAlsxtBloodAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	
	if (Attribute == GetCurrentBloodAttribute())
	{
		CheckMaxReachedForAttribute(MaximumBlood, AlsxtGASGameplayTags::State::TAG_State_Max_Blood.GetTag(), NewValue);
		return;
	}
	
	if (Attribute == GetBleedingAttribute())
	{
		CheckStatusTagForAttribute(AlsxtGASGameplayTags::Statuses::TAG_Debuff_Bleeding, NewValue, OldValue);
		return;
	}

	if (Attribute == GetMaximumBloodAttribute())
	{
		AdjustAttributeForMaxChange(GetCurrentBloodAttribute(), OldValue, NewValue);
		return;
	}
}

void UAlsxtBloodAttributeSet::PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const
{
	Super::PostAttributeBaseChange(Attribute, OldValue, NewValue);
}

void UAlsxtBloodAttributeSet::ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetMaximumBloodAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.f);
		return;
	}
	if (Attribute == GetCurrentBloodAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaximumBlood());
		return;
	}
	if (Attribute == GetBleedingAttribute() || Attribute == GetBleedHealingAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.f);
		return;
	}
}


