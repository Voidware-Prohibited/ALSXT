#pragma once

#include "NativeGameplayTags.h"
#include "AlsxtChooserParticleCriteria.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtChooserBreathParticleCriteria
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (Categories = "Als.Voice Variant, Als.Breath Type", AllowPrivateAccess))
	FGameplayTagContainer BreathType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (AllowPrivateAccess))
	float Stamina {0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (AllowPrivateAccess))
	float Exhaustion {0.0f};

	bool operator==(const FAlsxtChooserBreathParticleCriteria& other) const
	{
		return (other.BreathType == BreathType) && (other.Stamina == Stamina) && (other.Exhaustion == Exhaustion);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtChooserFootstepParticleCriteria
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (Categories = "Als.Voice Variant, Als.Breath Type", AllowPrivateAccess))
	FGameplayTagContainer BreathType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (AllowPrivateAccess))
	float Stamina {0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (AllowPrivateAccess))
	float Exhaustion {0.0f};

	bool operator==(const FAlsxtChooserFootstepParticleCriteria& other) const
	{
		return (other.BreathType == BreathType) && (other.Stamina == Stamina) && (other.Exhaustion == Exhaustion);
	}
};