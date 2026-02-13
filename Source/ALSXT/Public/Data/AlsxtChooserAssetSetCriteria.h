#pragma once

#include "NativeGameplayTags.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "AlsxtChooserAssetSetCriteria.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtFootstepAssetCriteria
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<UPhysicalMaterial> PhysicalMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (Categories = "Als.Voice Variant, Als.Breath Type", AllowPrivateAccess))
	FGameplayTagContainer Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (AllowPrivateAccess))
	float Weight {0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (AllowPrivateAccess))
	float Exhaustion {0.0f};

	bool operator==(const FAlsxtFootstepAssetCriteria& other) const
	{
		return (other.Type == Type) && (other.Weight == Weight) && (other.Exhaustion == Exhaustion);
	}
};