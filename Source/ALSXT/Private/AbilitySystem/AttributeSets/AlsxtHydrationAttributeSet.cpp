// MIT


#include "AbilitySystem/AttributeSets/AlsxtHydrationAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/Data/AlsxtGASGameplayTags.h"
#include "Net/UnrealNetwork.h"

UAlsxtHydrationAttributeSet::UAlsxtHydrationAttributeSet()
{
	MaximumHydration = 1.0f;
	CurrentHydration = 1.0f;
	HydrationRegeneration = 0.0f;
	Bleeding = 0.f;
	BleedHealing = 0.f;
}

void UAlsxtHydrationAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params{};
	Params.bIsPushBased = true;
	Params.Condition = COND_None;

	// Replicated to all
	DOREPLIFETIME_WITH_PARAMS_FAST(UAlsxtHydrationAttributeSet, CurrentHydration, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAlsxtHydrationAttributeSet, MaximumHydration, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAlsxtHydrationAttributeSet, Bleeding, Params);

	// Only Owner
	Params.Condition = COND_OwnerOnly;
	DOREPLIFETIME_WITH_PARAMS_FAST(UAlsxtHydrationAttributeSet, HydrationRegeneration, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAlsxtHydrationAttributeSet, BleedHealing, Params);
}

void UAlsxtHydrationAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
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

void UAlsxtHydrationAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// Store a local copy of the amount of Damage done and clear the Damage attribute.
		const float LocalDamageDone = GetDamage();

		SetDamage(0.f);
	
		if (LocalDamageDone > 0.0f)
		{
			// Apply the Hydration change and then clamp it.
			const float NewHydration = GetCurrentHydration() - LocalDamageDone;

			SetCurrentHydration(FMath::Clamp(NewHydration, 0.0f, GetMaximumHydration()));
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
			// Apply the Hydration change and then clamp it.
			const float NewHydration = GetCurrentHydration() + LocalHealingDone;

			SetCurrentHydration(FMath::Clamp(NewHydration, 0.0f, GetMaximumHydration()));
		}
		return;
	}

	if (Data.EvaluatedData.Attribute == GetHydrationRegenerationAttribute())
	{
		SetHydrationRegeneration(FMath::Clamp(GetHydrationRegeneration(), 0.0f, GetMaximumHydration()));
		return;
	}
}

void UAlsxtHydrationAttributeSet::OnRep_CurrentHydration(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlsxtHydrationAttributeSet, CurrentHydration, OldValue);
}

void UAlsxtHydrationAttributeSet::OnRep_MaximumHydration(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlsxtHydrationAttributeSet, MaximumHydration, OldValue);
}

void UAlsxtHydrationAttributeSet::OnRep_HydrationRegeneration(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlsxtHydrationAttributeSet, HydrationRegeneration, OldValue);
}

void UAlsxtHydrationAttributeSet::OnRep_Bleeding(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlsxtHydrationAttributeSet, Bleeding, OldValue);
}

void UAlsxtHydrationAttributeSet::OnRep_BleedHealing(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlsxtHydrationAttributeSet, BleedHealing, OldValue);
}

void UAlsxtHydrationAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	
	if (Attribute == GetCurrentHydrationAttribute())
	{
		CheckMaxReachedForAttribute(MaximumHydration, AlsxtGASGameplayTags::State::TAG_State_Max_Hydration.GetTag(), NewValue);
		return;
	}
	
	if (Attribute == GetBleedingAttribute())
	{
		CheckStatusTagForAttribute(AlsxtGASGameplayTags::Statuses::TAG_Debuff_Bleeding, NewValue, OldValue);
		return;
	}

	if (Attribute == GetMaximumHydrationAttribute())
	{
		AdjustAttributeForMaxChange(GetCurrentHydrationAttribute(), OldValue, NewValue);
		return;
	}
}

void UAlsxtHydrationAttributeSet::PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const
{
	Super::PostAttributeBaseChange(Attribute, OldValue, NewValue);
}

void UAlsxtHydrationAttributeSet::ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetMaximumHydrationAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.f);
		return;
	}
	if (Attribute == GetCurrentHydrationAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaximumHydration());
		return;
	}
	if (Attribute == GetBleedingAttribute() || Attribute == GetBleedHealingAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.f);
		return;
	}
}


