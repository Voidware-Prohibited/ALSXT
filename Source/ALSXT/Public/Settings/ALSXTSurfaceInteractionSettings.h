#pragma once

#include "Engine/DataAsset.h"
#include "Engine/EngineTypes.h"
#include "Utility/ALSXTStructs.h"
#include "ALSXTSurfaceInteractionSettings.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTSurfaceInteractionParameters
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FGameplayTag Location{FGameplayTag::EmptyTag};
};

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UALSXTSurfaceInteractionSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (TitleProperty = "{ElementalInteractions}", ForceInlineRow))
	TMap<TEnumAsByte<EPhysicalSurface>, FALSXTElementalInteractionSet> Interactions;
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTGeneralSurfaceInteractionSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAllowSurfaceInteractions {true};
};
