#pragma once

#include "AlsxtOverlayObjectLookupTableDataAsset.generated.h"

class UAlsxtOverlayDataAsset;
struct FGameplayTag;

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UAlsxtOverlayObjectLookupTableDataAsset: public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.OverlayMode"))
	TMap<FGameplayTag, TSoftObjectPtr<UAlsxtOverlayDataAsset> > OverlayObjectDataMap;
};