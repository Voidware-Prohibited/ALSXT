#pragma once

#include "AlsxtCameraEffectsSettings.h"
#include "AlsxtMovementSettings.h"
#include "AlsxtViewSettings.h"
#include "GameplayTagContainer.h"
#include "Camera/CameraShakeBase.h"
#include "Animation/AnimInstance.h"
#include "Utility/AlsxtOverlayStructs.h"
#include "Settings/AlsxtCameraEffectsSettings.h"
#include "Chooser.h"
#include "Components/Mesh/AlsxtPaintableSkeletalMeshComponent.h"
#include "Components/Mesh/AlsxtPaintableStaticMeshComponent.h"
#include "AlsxtOverlayObjectSettings.generated.h"

class UChooserTable;
struct FAlsxtMovementCameraShakeSettings;

UCLASS()
class ALSXT_API UAlsxtGeneralOverlayObjectSettingsDataAsset: public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ThrowingSocketName {"Throwing"};
};

UCLASS()
class ALSXT_API UAlsxtGeneralAimableOverlayObjectSettingsDataAsset: public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.OverlayMode"))
	FGameplayTagContainer AvailableAimTargetingModes;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.OverlayMode"))
	FGameplayTag DefaultCarryPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.OverlayMode"))
	FGameplayTag DefaultReadyPosition;
};

UCLASS()
class ALSXT_API UAlsxtAimableOverlayObjectSettingsDataAsset: public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.OverlayMode"))
	FGameplayTagContainer AvailableCarryPositions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.OverlayMode"))
	FGameplayTag DefaultCarryPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.OverlayMode"))
	FGameplayTagContainer AvailableReadyPositions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.OverlayMode"))
	FGameplayTagContainer AvailableOverlaySlots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.OverlayMode"))
	FGameplayTag DefaultReadyPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.OverlayMode"))
	FGameplayTagContainer AvailableTargetingModes;
};


USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtOverlayObjectSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UAlsxtPaintableSkeletalMeshComponent> SkeletalMesh {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UAlsxtPaintableStaticMeshComponent> StaticMesh {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UAlsxtPaintableStaticMeshComponent> PickupMesh {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseWeight {0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanBeThrown {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.OverlayMode"))
	FGameplayTag OverlayMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FootstepCameraShakeSet {0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<UPhysicalMaterial> MovementSoundsPhysicalMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovementSoundsPriority {0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<UPhysicalMaterial> MovementAccentSoundsPhysicalMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovementAccentSoundsPriority {0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<UPhysicalMaterial> ObjectMovementSoundsPhysicalMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ObjectMovementSoundsPriority {0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.OverlayMode"))
	FGameplayTagContainer AvailableStances;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.OverlayMode"))
	FGameplayTagContainer AvailableReady;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.OverlayMode"))
	FGameplayTagContainer AvailableWeaponReady;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.OverlayMode"))
	FGameplayTagContainer AvailableGaits;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Locmotion Variant", EditCondition = "bOverrideLocomotionVariant"))
	FGameplayTag LocomotionVariant;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bOverrideLocomotionVariant {false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ForceInlineRow))
	FAlsxtMovementRotationModeSettings MovementSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer AimableOverlayModes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseGunBoneForOverlayObjects {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer LeftIKOverlayModes;

	// General Settings for Gameplay Camera System
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ForceInlineRow))
	TSoftObjectPtr<UAlsxtViewCameraSettings> ViewCameraSettings {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGameplayTag, TObjectPtr<UAlsxtCameraShakeSettings>> CameraShake;
};


UCLASS()
class ALSXT_API UAlsxtOverlayObjectSettingsDataAsset: public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.OverlayMode"))
	TMap<FGameplayTag, FAlsxtOverlayAnimationInfo> Overlays;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAlsxtOverlayObjectSettings OverlayObjectSettings;
};