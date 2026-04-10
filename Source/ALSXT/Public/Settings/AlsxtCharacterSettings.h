// MIT

#pragma once

#include "CoreMinimal.h"
#include "AlsxtLocomotionSettings.h"
#include "AlsxtStatusEffectSettings.h"
#include "AlsxtViewSettings.h"
#include "Settings/AlsCharacterSettings.h"
#include "Settings/AlsxtLocomotionActionSettings.h"
#include "Settings/AlsxtOverlaySettings.h"
#include "Settings/AlsxtDefensiveModeSettings.h"
#include "Settings/AlsxtStatusSettings.h"
#include "Settings/AlsxtCharacterBreathEffectsSettings.h"
#include "Settings/AlsxtFootstepEffectSettings.h"
#include "Settings/AlsxtFreelookSettings.h"
#include "Settings/AlsxtHeldItemSettings.h"
#include "Settings/AlsxtProceduralAimSettings.h"
#include "Settings/AlsxtForegripPositionSettings.h"
#include "Settings/AlsxtVaultingSettings.h"
#include "Settings/AlsxtSlidingSettings.h"
#include "Settings/AlsxtFPEyeFocusSettings.h"
#include "Settings/AlsxtMeshRenderSettings.h"
#include "Settings/AlsxtMeshPaintingSettings.h"
#include "AlsxtCharacterSettings.generated.h"

UCLASS()
class ALSXT_API UAlsxtCharacterSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	// Movement Speed for Gaits and Stances
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", Meta = (ForceInlineRow))
	TSoftObjectPtr<UAlsxtMovementSettings> MovementSettings {nullptr};

	// Movement Speed for Gaits and Stances
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", Meta = (ForceInlineRow))
	TSoftObjectPtr<UAlsxtMovementSettings> ControlSettings {nullptr};

	// Movement Speed for Gaits and Stances
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", Meta = (ForceInlineRow))
	TSoftObjectPtr<UAlsxtMovementSettings> CameraSettings {nullptr};

	// Movement Speed for Gaits and Stances
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", Meta = (ForceInlineRow))
	TSoftObjectPtr<UAlsxtMovementSettings> SoundSettings {nullptr};
	
	// Settings for Base Locomotion, Locomotion Modifiers, Locomotion Status Effects
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects", Meta = (ForceInlineRow))
	TSoftObjectPtr<UAlsxtLocomotionSettingsDataAsset> LocomotionSettingsNew {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects", Meta = (ForceInlineRow))
	TSoftObjectPtr<UAlsxtOverlaySettingsDataAsset> OverlaySettingsNew {nullptr};
	
	// General Settings for Rolling, Sliding, Mantling and Vaulting
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ForceInlineRow))
	TSoftObjectPtr<UAlsxtLocomotionActionSettingsDataAsset> LocomotionActionSettings {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects", Meta = (ForceInlineRow))
	TSoftObjectPtr<UAlsxtStatusEffectSettingsDataAsset> StatusEffectSettings {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects", Meta = (ForceInlineRow))
	TSoftObjectPtr<UAlsxtGeneralViewSettingsDataAsset> ViewSettings {nullptr};

	// Old Code Below
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	FAlsxtLocomotionSettings LocomotionSettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	FAlsxtOverlaySettings OverlaySettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	FAlsxtDefensiveModeSettings DefensiveMode;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	FAlsxtStatusSettings StatusSettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	FAlsxtCharacterBreathEffectsSettings BreathEffects;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	FAlsxtGeneralFootstepEffectSettings FootstepEffects;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "First Person")
	FAlsxtFPEyeFocusSettings FirstPersonEyeFocus;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Freelook")
	FAlsxtGeneralFreelookSettings Freelook;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Actions")
	FAlsxtGeneralVaultingSettings Vaulting;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Actions")
	FAlsxtGeneralSlidingSettings Sliding;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	FAlsxtGeneralMeshRenderSettings MeshRender;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	FAlsxtGlobalGeneralMeshPaintingSettings MeshPainting;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Held Item")
	FAlsxtGeneralHeldItemSettings HeldItem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Procedural")
	FAlsxtProceduralAimSettings ProceduralAim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Procedural")
	FAlsxtForegripPositionSettings ForegripPosition;

	UAlsxtCharacterSettings();
	
};
