#pragma once

#include "Engine/DataAsset.h"
#include "Utility/AlsxtAnimationStructs.h"
#include "AlsxtLocomotionAnimationAssetsSettings.generated.h"

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UAlsxtLocomotionAnimationAssetsSettings : public UDataAsset
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ShowOnlyInnerProperties))
	FAlsxtLocomotionAnimationInstanceAssets Assets;

public:
	UAlsxtLocomotionAnimationAssetsSettings();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
