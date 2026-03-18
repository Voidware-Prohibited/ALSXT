#pragma once

#include "AlsxtOverlayLookupTableDataAsset.generated.h"

class UAlsxtOverlayDataAsset;
struct FGameplayTag;

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UAlsxtOverlayLookupTableDataAsset: public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.OverlayMode"))
	TMap<FGameplayTag, TSoftObjectPtr<UAlsxtOverlayDataAsset> > OverlayDataMap;
};