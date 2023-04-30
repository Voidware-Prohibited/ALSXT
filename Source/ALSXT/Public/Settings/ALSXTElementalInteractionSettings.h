#pragma once

#include "Engine/DataAsset.h"
#include "Engine/EngineTypes.h"
#include "Utility/ALSXTStructs.h"
#include "ALSXTElementalInteractionSettings.generated.h"

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UALSXTElementalInteractionSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (TitleProperty = "{ElementalInteractions}", ForceInlineRow))
	TMap<TEnumAsByte<EPhysicalSurface>, FALSXTElementalInteractionSet> Interactions;
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