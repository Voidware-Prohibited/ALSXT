#pragma once

#include "Engine/DataAsset.h"

#include "AlsxtChooserCameraShakeResultAssets.generated.h"

class UCameraShakeBase;

UCLASS(Blueprintable, BlueprintType)
class UAlsxtChooserCameraShakeResultAsset: public UDataAsset
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (AllowPrivateAccess))
	TSoftObjectPtr<UCameraShakeBase> CameraShake;
	
};
