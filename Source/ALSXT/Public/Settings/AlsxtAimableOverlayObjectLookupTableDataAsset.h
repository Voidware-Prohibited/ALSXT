#pragma once

#include "AlsxtAimableOverlayObjectLookupTableDataAsset.generated.h"

class UAlsxtAimableOverlayObjectSettingsDataAsset;
struct FGameplayTag;

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UAlsxtAimableOverlayObjectLookupTableDataAsset: public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.OverlayMode"))
	TMap<FGameplayTag, TSoftObjectPtr<UAlsxtAimableOverlayObjectSettingsDataAsset> > AimableOverlayObjectDataMap;
}; 