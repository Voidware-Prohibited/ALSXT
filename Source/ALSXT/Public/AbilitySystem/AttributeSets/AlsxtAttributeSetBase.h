// MIT

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AlsxtAttributeSetBase.generated.h"

// Use macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

struct FGameplayTag;

UCLASS()
class ALSXT_API UAlsxtAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()
	
protected:
	void AdjustAttributeForMaxChange(const FGameplayAttribute& AffectedAttribute, const float OldMaxValue, const float NewMaxValue) const;

	void CheckMaxReachedForAttribute(const FGameplayAttributeData& MaxAttribute, const FGameplayTag& MaxTag, const float& NewValue) const;
	void CheckMinReachedForAttribute(const FGameplayAttributeData& MinAttribute, const FGameplayTag& MinTag, const float& NewValue) const;
	void CheckStatusTagForAttribute(const FGameplayTag& StatusTag, const float& NewValue, const float& OldValue) const;


	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const {};
	
	
};
