#pragma once

#include "Utility/AlsxtStructs.h"
#include "Utility/AlsxtGameplayTags.h"
#include "NativeGameplayTags.h"
#include "AlsxtStatusEffectSettings.generated.h"

UCLASS()
class ALSXT_API UAlsxtStatusEffectSettingsDataAsset: public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ForceInlineRow))
	bool LocomotionActionSettings {false};
};