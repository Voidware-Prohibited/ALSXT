#pragma once

#include "AlsxtMovementSettings.h"
#include "GameplayTagContainer.h"
#include "Camera/CameraShakeBase.h"
#include "Animation/AnimInstance.h"
#include "Utility/AlsxtOverlayStructs.h"
#include "Settings/AlsxtCameraEffectsSettings.h"
#include "Chooser.h"
#include "AlsxtEquipmentSettings.generated.h"

struct FAlsxtMovementCameraShakeSettings;

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtEquipmentSettings
{
	GENERATED_BODY()

	// Multiple slots allow for example: a Helmet to occupy the Eyewear, Earwear slots.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories="Als.OverlayMode"))
	FGameplayTagContainer EquipmentSlots;

	// If occupying multiple slots, which slot should be considered "Primary"
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (EditCondition="EquipmentSlots.Num() > 1", Categories="Als.OverlayMode"))
	FGameplayTagContainer PrimarySlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UAlsxtPaintableSkeletalMeshComponent> SkeletalMesh {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UAlsxtPaintableStaticMeshComponent> StaticMesh {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UAlsxtPaintableStaticMeshComponent> PickupMesh {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.OverlayMode"))
	FGameplayTag OverlayMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseWeight {0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FootstepCameraShakeSet {0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovementActionSet {0.0f};

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
	TMap<FGameplayTag, TObjectPtr<UAlsxtCameraShakeSettings>> CameraShake;
};

UCLASS()
class ALSXT_API UAlsxtEquipmementSettingsDataAsset: public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.OverlayMode"))
	FAlsxtEquipmentSettings EquipmentSettings;
};

