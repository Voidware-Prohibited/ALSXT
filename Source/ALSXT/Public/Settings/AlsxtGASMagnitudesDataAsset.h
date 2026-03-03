// MIT

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AlsxtGASMagnitudesDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class ALSXT_API UAlsxtGASMagnitudesDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	// Maps a tag to a Value (e.g., -0.01)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stamina", meta=(ForceInlineRow="true"))
	TMap<FGameplayTag, float> MagnitudeTags;
};
