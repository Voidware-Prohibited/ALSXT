// MIT

#pragma once

#include "CoreMinimal.h"
#include "Settings/AlsCharacterSettings.h"
#include "Settings/ALSXTOverlaySettings.h"
#include "Settings/ALSXTStatusSettings.h"
#include "Settings/ALSXTCharacterBreathEffectsSettings.h"
#include "Settings/ALSXTFootstepEffectSettings.h"
#include "Settings/ALSXTFreelookSettings.h"
#include "Settings/ALSXTProceduralAimSettings.h"
#include "Settings/ALSXTForegripPositionSettings.h"
#include "Settings/ALSXTVaultingSettings.h"
#include "Settings/ALSXTSlidingSettings.h"
#include "Settings/ALSXTFPEyeFocusSettings.h"
#include "Settings/ALSXTMeshRenderSettings.h"
#include "Settings/ALSXTMeshPaintingSettings.h"
#include "ALSXTCharacterSettings.generated.h"

UCLASS()
class ALSXT_API UALSXTCharacterSettings : public UAlsCharacterSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	FALSXTOverlaySettings OverlaySettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	FALSXTStatusSettings StatusSettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	FALSXTCharacterBreathEffectsSettings BreathEffects;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	FALSXTGeneralFootstepEffectSettings FootstepEffects;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Freelook")
	FALSXTGeneralFreelookSettings Freelook;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Procedural")
	FALSXTProceduralAimSettings ProceduralAim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Procedural")
	FALSXTForegripPositionSettings ForegripPosition;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Actions")
	FALSXTGeneralVaultingSettings Vaulting;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Actions")
	FALSXTGeneralSlidingSettings Sliding;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "First Person")
	FALSXTFPEyeFocusSettings FirstPersonEyeFocus;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	FALSXTGeneralALSXTMeshRenderSettings MeshRender;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	FALSXTGlobalGeneralMeshPaintingSettings MeshPainting;

	UALSXTCharacterSettings();
	
};
