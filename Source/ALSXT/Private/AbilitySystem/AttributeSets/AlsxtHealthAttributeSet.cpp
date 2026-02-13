// MIT


#include "AbilitySystem/AttributeSets/AlsxtHealthAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/Data/AlsxtGASGameplayTags.h"
#include "Net/UnrealNetwork.h"

UAlsxtHealthAttributeSet::UAlsxtHealthAttributeSet()
{
	MaximumHealth = 1.0f;
	CurrentHealth = 1.0f;
	HealthRegeneration = 0.0f;
	Bleeding = 0.f;
	BleedHealing = 0.f;
}

void UAlsxtHealthAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params{};
	Params.bIsPushBased = true;
	Params.Condition = COND_None;

	// Replicated to all
	DOREPLIFETIME_WITH_PARAMS_FAST(UAlsxtHealthAttributeSet, CurrentHealth, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAlsxtHealthAttributeSet, MaximumHealth, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAlsxtHealthAttributeSet, Bleeding, Params);

	// Only Owner
	Params.Condition = COND_OwnerOnly;
	DOREPLIFETIME_WITH_PARAMS_FAST(UAlsxtHealthAttributeSet, HealthRegeneration, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAlsxtHealthAttributeSet, BleedHealing, Params);
}

void UAlsxtHealthAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
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

void UAlsxtHealthAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// Store a local copy of the amount of Damage done and clear the Damage attribute.
		const float LocalDamageDone = GetDamage();

		SetDamage(0.f);
	
		if (LocalDamageDone > 0.0f)
		{
			// Apply the Health change and then clamp it.
			const float NewHealth = GetCurrentHealth() - LocalDamageDone;

			SetCurrentHealth(FMath::Clamp(NewHealth, 0.0f, GetMaximumHealth()));
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
			// Apply the Health change and then clamp it.
			const float NewHealth = GetCurrentHealth() + LocalHealingDone;

			SetCurrentHealth(FMath::Clamp(NewHealth, 0.0f, GetMaximumHealth()));
		}
		return;
	}

	if (Data.EvaluatedData.Attribute == GetHealthRegenerationAttribute())
	{
		SetHealthRegeneration(FMath::Clamp(GetHealthRegeneration(), 0.0f, GetMaximumHealth()));
		return;
	}
}

void UAlsxtHealthAttributeSet::OnRep_CurrentHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlsxtHealthAttributeSet, CurrentHealth, OldValue);
}

void UAlsxtHealthAttributeSet::OnRep_MaximumHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlsxtHealthAttributeSet, MaximumHealth, OldValue);
}

void UAlsxtHealthAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlsxtHealthAttributeSet, HealthRegeneration, OldValue);
}

void UAlsxtHealthAttributeSet::OnRep_Bleeding(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlsxtHealthAttributeSet, Bleeding, OldValue);
}

void UAlsxtHealthAttributeSet::OnRep_BleedHealing(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlsxtHealthAttributeSet, BleedHealing, OldValue);
}

void UAlsxtHealthAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	
	if (Attribute == GetCurrentHealthAttribute())
	{
		CheckMaxReachedForAttribute(MaximumHealth, AlsxtGASGameplayTags::State::TAG_State_Max_Health.GetTag(), NewValue);
		return;
	}
	
	if (Attribute == GetBleedingAttribute())
	{
		CheckStatusTagForAttribute(AlsxtGASGameplayTags::Statuses::TAG_Debuff_Bleeding, NewValue, OldValue);
		return;
	}

	if (Attribute == GetMaximumHealthAttribute())
	{
		AdjustAttributeForMaxChange(GetCurrentHealthAttribute(), OldValue, NewValue);
		return;
	}
}

void UAlsxtHealthAttributeSet::PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const
{
	Super::PostAttributeBaseChange(Attribute, OldValue, NewValue);
}

void UAlsxtHealthAttributeSet::ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetMaximumHealthAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.f);
		return;
	}
	if (Attribute == GetCurrentHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaximumHealth());
		return;
	}
	if (Attribute == GetBleedingAttribute() || Attribute == GetBleedHealingAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.f);
		return;
	}
}


