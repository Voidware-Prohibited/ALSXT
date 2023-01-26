// MIT

#pragma once

#include "CoreMinimal.h"
#include "Settings/AlsCharacterSettings.h"
#include "Settings/ALSXTSlidingSettings.h"
#include "Settings/ALSXTFPEyeFocusSettings.h"
#include "Settings/ALSXTImpactReactionSettings.h"
#include "Settings/ALSXTUnarmedCombatSettings.h"
#include "ALSXTCharacterSettings.generated.h"

UENUM(BlueprintType)
enum class ESide : uint8
{
	Message UMETA(DisplayName = "Left"),
	Warning UMETA(DisplayName = "Right"),
	Count UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(ESide, ESide::Count);

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FALSXTImpactReactionSettings ImpactReaction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FALSXTUnarmedCombatSettings UnarmedCombat;

	UALSXTCharacterSettings();
	
};
