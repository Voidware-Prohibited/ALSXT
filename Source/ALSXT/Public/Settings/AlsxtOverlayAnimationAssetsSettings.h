#pragma once

#include "Engine/DataAsset.h"
#include "Utility/AlsxtAnimationStructs.h"
#include "AlsxtOverlayAnimationAssetsSettings.generated.h"

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UAlsxtOverlayAnimationAssetsSettings : public UDataAsset
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (ShowOnlyInnerProperties))
	FAlsxtOverlayAnimationInstanceAssets Assets;

public:
	UAlsxtOverlayAnimationAssetsSettings();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
