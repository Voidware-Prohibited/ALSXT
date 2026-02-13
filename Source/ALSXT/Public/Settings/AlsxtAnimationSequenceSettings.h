#pragma once

#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AlsxtAnimationSequenceSettings.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtPoseMapSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace")
	float MinimumSpeedForWallRun { 500.0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montages")
	bool MirrorSequence { false };
};

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UAlsxtPoseMapSettingsAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Features")
	TMap<FGameplayTag, FAlsxtPoseMapSettings> PoseMap;
};
