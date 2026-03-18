#pragma once
#include "AlsxtAnimationSequenceSettings.h"
#include "AlsxtMovementSettings.h"
#include "Chooser.h"
#include "Utility/AlsxtAnimationStructs.h"

#include "AlsxtLocomotionSettings.generated.h"

class UAlsxtAimableOverlayObjectLookupTableDataAsset;
class UAlsxtOverlayObjectLookupTableDataAsset;
class UAlsxtOverlayLookupTableDataAsset;
struct FGameplayTag;

UCLASS()
class ALSXT_API UAlsxtLocomotionVariantSettingsDataAsset: public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Locomotion Variants", Meta = (ForceInlineRow, Categories = "Als.Locomotion Variant"))
	TMap<FGameplayTag, FAlsxtLocomotionVariantInfo> VariantSettings;
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtLocomotionSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (Categories = "Als.Locomotion Variant"))
	TSoftObjectPtr<UAlsxtLocomotionVariantSettingsDataAsset> LocomotionVariantSettings {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool bEnableCombatStanceSwitching {true};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Chooser Tables")
	TSoftObjectPtr<UChooserTable> LocomotionAssetsChooserTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Chooser Tables")
	TSoftObjectPtr<UChooserTable> LocomotionActionAssetsChooserTable;

	// Locomotion Variant the Character will start in
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults",  Meta = (Categories = "Als.Locomotion Variant"))
	FGameplayTag DefaultLocomotionVariant {ALSXTLocomotionVariantTags::Default};

	// Combat Stance the Character will start in
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults", Meta = (Categories = "Als.Combat Stance"))
	FGameplayTag DefaultCombatStance {ALSXTCombatStanceTags::Orthodox};
	
};

UCLASS()
class ALSXT_API UAlsxtLocomotionSettingsDataAsset: public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ForceInlineRow))
	FAlsxtLocomotionSettings LocomotionSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TSoftObjectPtr<UAlsxtOverlayLookupTableDataAsset> OverlayLookupTable {nullptr};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TSoftObjectPtr<UAlsxtOverlayObjectLookupTableDataAsset> OverlayObjectLookupTable {nullptr};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TSoftObjectPtr<UAlsxtAimableOverlayObjectLookupTableDataAsset> AimableOverlayObjectLookupTable {nullptr};
};

