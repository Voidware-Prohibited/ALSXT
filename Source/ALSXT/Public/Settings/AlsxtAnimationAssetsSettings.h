#pragma once

#include "Engine/DataAsset.h"
#include "Utility/AlsxtAnimationStructs.h"
#include "AlsxtAnimationAssetsSettings.generated.h"

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UAlsxtAnimationAssetsSettings : public UDataAsset
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ShowOnlyInnerProperties))
	FAlsxtAnimationInstanceAssets Assets;

public:
	UAlsxtAnimationAssetsSettings();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
