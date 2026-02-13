#pragma once

#include "NativeGameplayTags.h"
#include "AlsxtChooserCameraShakeCriteria.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtChooserMovementCameraShakeCriteria
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (Categories = "Als.Voice Variant, Als.Breath Type", AllowPrivateAccess))
	FGameplayTagContainer BreathType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (AllowPrivateAccess))
	float Stamina {0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (AllowPrivateAccess))
	float Exhaustion {0.0f};

	bool operator==(const FAlsxtChooserMovementCameraShakeCriteria& other) const
	{
		return (other.BreathType == BreathType) && (other.Stamina == Stamina) && (other.Exhaustion == Exhaustion);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtChooserFootstepCameraShakeCriteria
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (Categories = "Als.Voice Variant, Als.Breath Type", AllowPrivateAccess))
	FGameplayTagContainer BreathType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (AllowPrivateAccess))
	float Stamina {0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (AllowPrivateAccess))
	float Exhaustion {0.0f};

	bool operator==(const FAlsxtChooserFootstepCameraShakeCriteria& other) const
	{
		return (other.BreathType == BreathType) && (other.Stamina == Stamina) && (other.Exhaustion == Exhaustion);
	}
};