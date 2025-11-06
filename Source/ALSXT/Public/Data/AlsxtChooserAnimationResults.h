#pragma once

#include "NativeGameplayTags.h"
#include "AlsxtChooserAnimationResults.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtChooserCriteriaLocomotion
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Criteria|Locomotion", Meta = (Categories = "Als.Locomotion Variant", AllowPrivateAccess))
	FGameplayTag LocomotionVariant;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Criteria|Locomotion", Meta = (Categories = "Als.Stance", AllowPrivateAccess))
	FGameplayTag Stance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Criteria|Locomotion", Meta = (Categories = "Als.Combat Stance", AllowPrivateAccess))
	FGameplayTag CombatStance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Criteria|Locomotion", Meta = (Categories = "Als.Injury", AllowPrivateAccess))
	FGameplayTag Injury;

	bool operator==(const FAlsxtChooserCriteriaLocomotion& other) const
	{
		return (other.LocomotionVariant == LocomotionVariant) && (other.Stance == Stance) && (other.CombatStance == CombatStance) && (other.Injury == Injury);
	}
};