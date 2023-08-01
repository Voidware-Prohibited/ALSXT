// MIT

#pragma once

#include "CoreMinimal.h"
#include "Settings/AlsCharacterSettings.h"
#include "Settings/ALSXTFootstepEffectSettings.h"
#include "Settings/ALSXTFreelookSettings.h"
#include "Settings/ALSXTProceduralAimSettings.h"
#include "Settings/ALSXTForegripPositionSettings.h"
#include "Settings/ALSXTVaultingSettings.h"
#include "Settings/ALSXTSlidingSettings.h"
#include "Settings/ALSXTFPEyeFocusSettings.h"
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
	FALSXTGeneralFootstepEffectSettings FootstepEffects;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FALSXTGeneralFreelookSettings Freelook;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FALSXTProceduralAimSettings ProceduralAim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FALSXTForegripPositionSettings ForegripPosition;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FALSXTGeneralVaultingSettings Vaulting;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FALSXTGeneralSlidingSettings Sliding;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FALSXTFPEyeFocusSettings FirstPersonEyeFocus;

	UALSXTCharacterSettings();
	
};
