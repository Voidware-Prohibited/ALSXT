#pragma once

#include "NativeGameplayTags.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Settings/AlsxtContentRatingSettings.h"

#include "AlsxtChooserSoundCriteria.generated.h"

enum class EIarcRating : FPlatformTypes::uint8;

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtChooserBreathSoundCriteria
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (Categories = "Als.Voice Variant, Als.Breath Type", AllowPrivateAccess))
	FGameplayTagContainer BreathType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (AllowPrivateAccess))
	float Stamina {0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (AllowPrivateAccess))
	float Exhaustion {0.0f};

	bool operator==(const FAlsxtChooserBreathSoundCriteria& other) const
	{
		return (other.BreathType == BreathType) && (other.Stamina == Stamina) && (other.Exhaustion == Exhaustion);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtChooserHoldingBreathSoundCriteria
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (Categories = "Als.Voice Variant,Als.Breath Type,Als.Holding Breath", AllowPrivateAccess))
	FGameplayTagContainer HoldingBreathType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (AllowPrivateAccess))
	float Stamina {0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (AllowPrivateAccess))
	float Exhaustion {0.0f};


	bool operator==(const FAlsxtChooserHoldingBreathSoundCriteria& other) const
	{
		return (other.HoldingBreathType == HoldingBreathType) && (other.Stamina == Stamina) && (other.Exhaustion == Exhaustion);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtChooserCharacterMovementSoundCriteria
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (AllowPrivateAccess))
	TSoftClassPtr<UPhysicalMaterial> PhysicalMaterial;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (Categories = "Als.Character Movement Sound,Als.OverlayMode", AllowPrivateAccess))
	FGameplayTagContainer Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (Categories = "Als.Object Weight", AllowPrivateAccess))
	float Weight {0.0f};

	bool operator==(const FAlsxtChooserCharacterMovementSoundCriteria& other) const
	{
		return (other.Type == Type) && (other.Weight == Weight);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtChooserCharacterMovementAccentSoundCriteria
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (AllowPrivateAccess))
	TSoftClassPtr<UPhysicalMaterial> PhysicalMaterial;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (Categories = "Als.Character Movement Sound,Als.OverlayMode", AllowPrivateAccess))
	FGameplayTagContainer Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (Categories = "Als.Object Weight", AllowPrivateAccess))
	float Weight {0.0f};

	bool operator==(const FAlsxtChooserCharacterMovementAccentSoundCriteria& other) const
	{
		return (other.Type == Type) && (other.Weight == Weight);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtChooserOverlayObjectSoundCriteria
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (AllowPrivateAccess))
	TSoftClassPtr<UPhysicalMaterial> PhysicalMaterial;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (Categories = "Als.Character Movement Sound,Als.OverlayMode", AllowPrivateAccess))
	FGameplayTagContainer Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (Categories = "Als.Object Weight", AllowPrivateAccess))
	float Weight {0.0f};

	bool operator==(const FAlsxtChooserOverlayObjectSoundCriteria& other) const
	{
		return (other.Type == Type) && (other.Weight == Weight);
	}
};

// Returns an Asset with an Array of Sound Assets
USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtChooserCharacterFootstepSoundCriteria
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (AllowPrivateAccess))
	TSoftClassPtr<UPhysicalMaterial> FootwearPhysicalMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (AllowPrivateAccess))
	TSoftClassPtr<UPhysicalMaterial> SurfacePhysicalMaterial;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (Categories = "Als.Character Movement Sound,Als.OverlayMode", AllowPrivateAccess))
	FGameplayTagContainer StepType;

	bool operator==(const FAlsxtChooserCharacterFootstepSoundCriteria& other) const
	{
		return (other.FootwearPhysicalMaterial == FootwearPhysicalMaterial) && (other.SurfacePhysicalMaterial == SurfacePhysicalMaterial) && (other.StepType == StepType);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtChooserCharacterActionSoundCriteria
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (Categories = "Als.Voice Variant,Als.OverlayMode,Als.Action Strength", AllowPrivateAccess))
	FGameplayTagContainer Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (AllowPrivateAccess))
	float Stamina {0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (AllowPrivateAccess))
	float Exhaustion {0.0f};

	bool operator==(const FAlsxtChooserCharacterActionSoundCriteria& other) const
	{
		return (other.Type == Type) && (other.Stamina == Stamina) && (other.Exhaustion == Exhaustion);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtChooserCharacterContextualSoundCriteria
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (Categories = "Als.Voice Variant,Als.OverlayMode,Als.Action Strength", AllowPrivateAccess))
	FGameplayTagContainer Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (AllowPrivateAccess))
	float Stamina {0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (AllowPrivateAccess))
	float Exhaustion {0.0f};

	bool operator==(const FAlsxtChooserCharacterActionSoundCriteria& other) const
	{
		return (other.Type == Type) && (other.Stamina == Stamina) && (other.Exhaustion == Exhaustion);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtChooserCharacterDamageSoundCriteria
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (Categories = "Als.Voice Variant,Als.Impact Form,Als.Attack Method", AllowPrivateAccess))
	FGameplayTagContainer Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (AllowPrivateAccess))
	float Damage {0.0f} ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (AllowPrivateAccess))
	float CurrentHealth {0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	EIarcRating IarcRating {EIarcRating::IarcRating_3};

	bool operator==(const FAlsxtChooserCharacterDamageSoundCriteria& other) const
	{
		return (other.Type == Type) && (other.Damage == Damage) && (other.CurrentHealth == CurrentHealth);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtChooserWeaponActionSoundCriteria
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (Categories = "Als.Weapon,Als.Weapon Action", AllowPrivateAccess))
	FGameplayTag Type;


	bool operator==(const FAlsxtChooserWeaponActionSoundCriteria& other) const
	{
		return (other.Type == Type);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtChooserCharacterImpactSoundCriteria
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (AllowPrivateAccess))
	TSoftClassPtr<UPhysicalMaterial> CharacterPhysicalMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (AllowPrivateAccess))
	TSoftClassPtr<UPhysicalMaterial> SurfacePhysicalMaterial;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (Categories = "Als.Character Movement Sound,Als.OverlayMode", AllowPrivateAccess))
	FGameplayTagContainer ImpactType;

	bool operator==(const FAlsxtChooserCharacterImpactSoundCriteria& other) const
	{
		return (other.CharacterPhysicalMaterial == CharacterPhysicalMaterial) && (other.SurfacePhysicalMaterial == SurfacePhysicalMaterial) && (other.ImpactType == ImpactType);
	}
};