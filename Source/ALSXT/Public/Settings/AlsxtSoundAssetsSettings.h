#pragma once

#include "Engine/DataAsset.h"
#include "Utility/AlsxtAnimationStructs.h"
#include "Utility/AlsxtStructs.h"
#include "AlsxtSoundAssetsSettings.generated.h"


UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UAlsxtSoundAssetsArraySettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ShowOnlyInnerProperties))
	TArray<FSound> Assets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (EditCondition="Assets.Num() > 1", ArrayClamp = "Assets"))
	int NoRepeat;

public:
	UAlsxtSoundAssetsArraySettings();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
