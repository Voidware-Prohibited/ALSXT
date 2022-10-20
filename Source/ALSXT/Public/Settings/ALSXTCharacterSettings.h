// MIT

#pragma once

#include "CoreMinimal.h"
#include "Settings/AlsCharacterSettings.h"
#include "Settings/ALSXTSlidingSettings.h"
#include "Settings/ALSXTFPEyeFocusSettings.h"
#include "ALSXTCharacterSettings.generated.h"

UENUM(BlueprintType)
enum class ESide : uint8
{
	Message UMETA(DisplayName = "Left"),
	Warning UMETA(DisplayName = "Right"),
};

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FALSXTFPEyeFocusSettings FirstPersonEyeFocus;

	UALSXTCharacterSettings();
	
};
