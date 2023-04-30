#pragma once

#include "Utility/ALSXTStructs.h"
#include "Engine/DataAsset.h"
#include "Engine/EngineTypes.h"
#include "ALSXTElementalInteractionSettings.generated.h"

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UALSXTElementalInteractionSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds|Impact", Meta = (TitleProperty = "{Sounds}", AllowPrivateAccess))
	FALSXTImpactSoundMap ImpactSounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds|Falling", Meta = (TitleProperty = "{Sounds}", AllowPrivateAccess))
	FALSXTImpactSoundMap BodyFallSounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles|Impact", Meta = (TitleProperty = "{Particles}", AllowPrivateAccess))
	FALSXTImpactParticleMap ImpactParticles;
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTGeneralElementalInteractionSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool EnableElementalInteractions{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool DebugMode{ false };
};