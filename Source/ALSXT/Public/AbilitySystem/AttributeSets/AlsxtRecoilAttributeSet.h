// Copyright (C) 2026 Uriel Ballinas, VOIDWARE Prohibited. All rights reserved.
// This software is licensed under the MIT License (LICENSE.md).

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AlsxtAttributeSetBase.h"
#include "AlsxtRecoilAttributeSet.generated.h"


// Contains Attributes related to Recoil. Any Character that uses Recoil will need this Attribute Set.
//
//	Contents:
//	Recoil - Primarily used as reduction / increase to damage or status
UCLASS()
class ALSXT_API UAlsxtRecoilAttributeSet : public UAlsxtAttributeSetBase
{
	GENERATED_BODY()

public:
	
	UAlsxtRecoilAttributeSet();

	static float RecoilStrength_BASE;
	static float RecoilStrength_MAX;
	static float RecoilStrength_MIN;

	static float RecoilClimb_BASE;
	static float RecoilClimb_MAX;
	static float RecoilClimb_MIN;

	static float RecoilDrift_BASE;
	static float RecoilDrift_MAX;
	static float RecoilDrift_MIN;

	static float RecoilStability_BASE;
	static float RecoilStability_MAX;
	static float RecoilStability_MIN;

	static float RecoilKickback_BASE;
	static float RecoilKickback_MAX;
	static float RecoilKickback_MIN;

	static float RecoilNoiseStrength_BASE;
	static float RecoilNoiseStrength_MAX;
	static float RecoilNoiseStrength_MIN;

	// Attribute Set Overrides.
	virtual void PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const override;

	virtual void ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const override;

	// Set Attributes to replicate.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Holds the current value for Recoil.
	UPROPERTY(BlueprintReadOnly, Category = "Recoil Attribute Set", ReplicatedUsing = OnRep_RecoilStrength)
	FGameplayAttributeData RecoilStrength;
	ATTRIBUTE_ACCESSORS_BASIC(UAlsxtRecoilAttributeSet, RecoilStrength)

	UPROPERTY(BlueprintReadOnly, Category = "Recoil Attribute Set", ReplicatedUsing = OnRep_RecoilClimb, meta = (DisplayName = "Climb"))
	FGameplayAttributeData RecoilClimb;
	ATTRIBUTE_ACCESSORS_BASIC(UAlsxtRecoilAttributeSet, RecoilClimb)

	UPROPERTY(BlueprintReadOnly, Category = "Recoil Attribute Set", ReplicatedUsing = OnRep_RecoilDrift, meta = (DisplayName = "Drift"))
	FGameplayAttributeData RecoilDrift;
	ATTRIBUTE_ACCESSORS_BASIC(UAlsxtRecoilAttributeSet, RecoilDrift)

	UPROPERTY(BlueprintReadOnly, Category = "Recoil Attribute Set", ReplicatedUsing = OnRep_RecoilStability, meta = (DisplayName = "Stability"))
	FGameplayAttributeData RecoilStability;
	ATTRIBUTE_ACCESSORS_BASIC(UAlsxtRecoilAttributeSet, RecoilStability)
	
	UPROPERTY(BlueprintReadOnly, Category = "Recoil Attribute Set", ReplicatedUsing = OnRep_RecoilKickback, meta = (DisplayName = "Kickback"))
    FGameplayAttributeData RecoilKickback;
    ATTRIBUTE_ACCESSORS_BASIC(UAlsxtRecoilAttributeSet, RecoilKickback)
	
	UPROPERTY(BlueprintReadOnly, Category = "Recoil Attribute Set", ReplicatedUsing = OnRep_RecoilNoiseStrength, meta = (DisplayName = "Noise"))
    FGameplayAttributeData RecoilNoiseStrength;
    ATTRIBUTE_ACCESSORS_BASIC(UAlsxtRecoilAttributeSet, RecoilNoiseStrength)

	UFUNCTION(BlueprintPure, Category = "Recoil Attribute Set")
	static float GetBaseRecoilStrength() { return RecoilStrength_BASE; }

	UFUNCTION(BlueprintPure, Category = "Recoil Attribute Set")
	static FVector2D GetMinMaxRecoilStrength() { return FVector2D(RecoilStrength_MIN, RecoilStrength_MAX); }

	UFUNCTION(BlueprintPure, Category = "Recoil Attribute Set")
	static float GetBaseRecoilClimb() { return RecoilClimb_BASE; }

	UFUNCTION(BlueprintPure, Category = "Recoil Attribute Set")
	static FVector2D GetMinMaxRecoilClimb() { return FVector2D(RecoilClimb_MIN, RecoilClimb_MAX); }

	UFUNCTION(BlueprintPure, Category = "Recoil Attribute Set")
	static float GetBaseRecoilDrift() { return RecoilDrift_BASE; }

	UFUNCTION(BlueprintPure, Category = "Recoil Attribute Set")
	static FVector2D GetMinMaxRecoilDrift() { return FVector2D(RecoilDrift_MIN, RecoilDrift_MAX); }

	UFUNCTION(BlueprintPure, Category = "Recoil Attribute Set")
	static float GetBaseRecoilStability() { return RecoilStability_BASE; }

	UFUNCTION(BlueprintPure, Category = "Recoil Attribute Set")
	static FVector2D GetMinMaxRecoilStability() { return FVector2D(RecoilStability_MIN, RecoilStability_MAX); }

	UFUNCTION(BlueprintPure, Category = "Recoil Attribute Set")
	static float GetBaseRecoilKickback() { return RecoilKickback_BASE; }

	UFUNCTION(BlueprintPure, Category = "Recoil Attribute Set")
	static FVector2D GetMinMaxRecoilKickback() { return FVector2D(RecoilKickback_MIN, RecoilKickback_MAX); }

	UFUNCTION(BlueprintPure, Category = "Recoil Attribute Set")
	static float GetBaseRecoilNoiseStrength() { return RecoilNoiseStrength_BASE; }

	UFUNCTION(BlueprintPure, Category = "Recoil Attribute Set")
	static FVector2D GetMinMaxRecoilNoiseStrength() { return FVector2D(RecoilNoiseStrength_MIN, RecoilNoiseStrength_MAX); }

protected:
	
	UFUNCTION()
	virtual void OnRep_RecoilStrength(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_RecoilClimb(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_RecoilDrift(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_RecoilStability(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_RecoilKickback(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_RecoilNoiseStrength(const FGameplayAttributeData& OldValue);
};