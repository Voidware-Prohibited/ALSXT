// MIT

#include "AbilitySystem/AttributeSets/AlsxtAttributeSetBase.h"
#include "AbilitySystemComponent.h"

void UAlsxtAttributeSetBase::AdjustAttributeForMaxChange(const FGameplayAttribute& AffectedAttribute, const float OldMaxValue, const float NewMaxValue) const
{
	UAbilitySystemComponent* const ASC = GetOwningAbilitySystemComponent();
	if (!ASC)
	{
		return;
	}
	
	if (OldMaxValue <= 0.f || FMath::IsNearlyEqual(OldMaxValue, NewMaxValue, 0.f))
	{
		return;
	}
	
	// Change current value to maintain the Current Value / Maximum Value percentage.
	ASC->SetNumericAttributeBase(AffectedAttribute, ASC->GetNumericAttributeBase(AffectedAttribute) * NewMaxValue / OldMaxValue);
}

void UAlsxtAttributeSetBase::CheckMaxReachedForAttribute(const FGameplayAttributeData& MaxAttribute, const FGameplayTag& MaxTag, const float& NewValue) const
{
	UAbilitySystemComponent* const ASC = GetOwningAbilitySystemComponent();
	if (!ASC)
	{
		return;
	}

	const float Max = MaxAttribute.GetCurrentValue();
	const bool bHasTag = ASC->HasMatchingGameplayTag(MaxTag);

	int32 Count = -1;
	if (NewValue < Max && bHasTag)
	{
		Count = 0;
	} else if (NewValue >= Max && !bHasTag)
	{
		Count = 1;
	}

	if (Count >= 0)
	{
		ASC->SetLooseGameplayTagCount(MaxTag, Count);
		// ASC->SetReplicatedLooseGameplayTagCount(MaxTag, Count);
	}
}

void UAlsxtAttributeSetBase::CheckMinReachedForAttribute(const FGameplayAttributeData& MinAttribute, const FGameplayTag& MinTag, const float& NewValue) const
{
	UAbilitySystemComponent* const ASC = GetOwningAbilitySystemComponent();
	if (!ASC)
	{
		return;
	}

	const float Min = MinAttribute.GetCurrentValue();
	const bool bHasTag = ASC->HasMatchingGameplayTag(MinTag);

	int32 Count = -1;
	if (Min < NewValue && bHasTag)
	{
		Count = 0;
	} else if (Min >= NewValue && !bHasTag)
	{
		Count = 1;
	}

	if (Count >= 0)
	{
		ASC->SetLooseGameplayTagCount(MinTag, Count);
		// ASC->SetReplicatedLooseGameplayTagCount(MinTag, Count);
	}
}

void UAlsxtAttributeSetBase::CheckStatusTagForAttribute(const FGameplayTag& StatusTag, const float& NewValue, const float& OldValue) const
{
	UAbilitySystemComponent* const ASC = GetOwningAbilitySystemComponent();
	if (!ASC)
	{
		return;
	}
	
	int32 Count = -1;
		
	if (NewValue <= 0.f && OldValue > 0.f)
	{
		Count = 0;
	}
	else if (NewValue > 0.f && OldValue <= 0.f)
	{
		Count = 1;
	}

	if (Count >= 0)
	{
		ASC->SetLooseGameplayTagCount(StatusTag, Count);
		// ASC->SetReplicatedLooseGameplayTagCount(StatusTag, Count);
	}
}

void UAlsxtAttributeSetBase::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	ClampAttributes(Attribute, NewValue);
}

void UAlsxtAttributeSetBase::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
	ClampAttributes(Attribute, NewValue);
}
