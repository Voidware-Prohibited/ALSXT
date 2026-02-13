// Copyright (C) 2026 Uriel Ballinas, VOIDWARE Prohibited. All rights reserved.
// This software is licensed under the MIT License (LICENSE.md).

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AlsxtAttributeSetBase.h"
#include "AlsxtEncumbranceAttributeSet.generated.h"


// Contains Attributes related to Encumbrance. Any Character that uses Encumbrance will need this Attribute Set.
//
//	Contents:
//	Encumbrance - Primarily used as reduction / increase to damage or status
UCLASS()
class ALSXT_API UAlsxtEncumbranceAttributeSet : public UAlsxtAttributeSetBase
{
	GENERATED_BODY()

public:
	
	UAlsxtEncumbranceAttributeSet();

	static float Encumbrance_BASE;
	static float Encumbrance_MAX;
	static float Encumbrance_MIN;

	// Attribute Set Overrides.
	virtual void PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const override;

	virtual void ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const override;

	// Set Attributes to replicate.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Holds the current value for Encumbrance.
	UPROPERTY(BlueprintReadOnly, Category = "Encumbrance Attribute Set", ReplicatedUsing = OnRep_Encumbrance)
	FGameplayAttributeData Encumbrance;
	ATTRIBUTE_ACCESSORS_BASIC(UAlsxtEncumbranceAttributeSet, Encumbrance)

	UFUNCTION(BlueprintPure, Category = "Encumbrance Attribute Set")
	static float GetBaseEncumbrance() { return Encumbrance_BASE; }

	UFUNCTION(BlueprintPure, Category = "Encumbrance Attribute Set")
	static FVector2D GetMinMaxEncumbrance() { return FVector2D(Encumbrance_MIN, Encumbrance_MAX); }

protected:
	
	UFUNCTION()
	virtual void OnRep_Encumbrance(const FGameplayAttributeData& OldValue);
};