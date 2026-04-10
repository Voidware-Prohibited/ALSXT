// MIT

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AlsxtAttributeSetBase.h"
#include "AlsxtMovementAttributeSet.generated.h"


// Contains Attributes related to Movement.
//
//	Contents:
UCLASS()
class ALSXT_API UAlsxtMovementAttributeSet : public UAlsxtAttributeSetBase
{
	GENERATED_BODY()

public:
	
	UAlsxtMovementAttributeSet();

	// Attribute Set Overrides.
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

	virtual void ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const override;

	// Set Attributes to replicate.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// The current movement speed (base + modifiers)
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MovementSpeed)
	FGameplayAttributeData MovementSpeed;
	ATTRIBUTE_ACCESSORS(UAlsxtMovementAttributeSet, MovementSpeed)

	UPROPERTY(BlueprintReadOnly, Category = "Movement Attribute Set", ReplicatedUsing = OnRep_MovementSpeedMultiplier)
	FGameplayAttributeData MovementSpeedMultiplier;
	ATTRIBUTE_ACCESSORS_BASIC(UAlsxtMovementAttributeSet, MovementSpeedMultiplier)

	UPROPERTY(BlueprintReadOnly, Category = "Movement Attribute Set", ReplicatedUsing = OnRep_MovementAccelerationMultiplier)
	FGameplayAttributeData MovementAccelerationMultiplier;
	ATTRIBUTE_ACCESSORS_BASIC(UAlsxtMovementAttributeSet, MovementAccelerationMultiplier)
	
protected:
	UFUNCTION()
	virtual void OnRep_MovementSpeed(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	virtual void OnRep_MovementSpeedMultiplier(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_MovementAccelerationMultiplier(const FGameplayAttributeData& OldValue);
};