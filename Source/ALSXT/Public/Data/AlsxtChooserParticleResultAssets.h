#pragma once

#include "Engine/DataAsset.h"

#include "AlsxtChooserParticleResultAssets.generated.h"

class UNiagaraSystem;

UCLASS(Blueprintable, BlueprintType)
class UAlsxtChooserParticleResultAsset: public UDataAsset
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (AllowPrivateAccess))
	TSoftObjectPtr<UNiagaraSystem> ParticleSystem;
	
};
