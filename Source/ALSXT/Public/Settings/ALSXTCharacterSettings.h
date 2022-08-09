// MIT

#pragma once

#include "CoreMinimal.h"
#include "Settings/AlsCharacterSettings.h"
#include "Settings/ALSXTSlidingSettings.h"
#include "ALSXTCharacterSettings.generated.h"

/**
 * 
 */
UCLASS()
class ALSXT_API UALSXTCharacterSettings : public UAlsCharacterSettings
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FALSXTSlidingSettings Sliding;

	UALSXTCharacterSettings();
	
};
