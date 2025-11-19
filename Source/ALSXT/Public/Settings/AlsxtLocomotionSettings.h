#pragma once
#include "Chooser.h"
#include "Utility/AlsxtAnimationStructs.h"

#include "AlsxtLocomotionSettings.generated.h"

struct FGameplayTag;

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtLocomotionSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Locomotion Variants")
	TSoftObjectPtr<UChooserTable> ChooserTable; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Locomotion Variants")
	TMap<FGameplayTag, FAlsxtLocomotionVariantInfo> VariantSettings;
};
