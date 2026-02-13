#pragma once

#include "Engine/DataAsset.h"
#include "Utility/AlsxtStructs.h"

#include "AlsxtChooserSoundResultAssets.generated.h"

struct FAlsxtSound;

UCLASS(Blueprintable, BlueprintType)
class UAlsxtChooserSoundResultAsset: public UDataAsset
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (AllowPrivateAccess))
	FAlsxtSound Sound;
	
};

UCLASS(Blueprintable, BlueprintType)
class UAlsxtChooserSoundsArrayResultAsset: public UDataAsset
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (AllowPrivateAccess))
	TArray<FAlsxtSound> Sounds;
	
};
