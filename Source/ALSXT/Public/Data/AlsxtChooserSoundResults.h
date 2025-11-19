#pragma once

#include "NativeGameplayTags.h"
#include "AlsxtChooserSoundResults.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtBreathChooserSoundResult
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (Categories = "Als.Voice Variant, Als.Breath Type", AllowPrivateAccess))
	FGameplayTagContainer BreathType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (AllowPrivateAccess))
	float Stamina {0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (AllowPrivateAccess))
	float Exhaustion {0.0f};

	bool operator==(const FAlsxtBreathChooserSoundResult& other) const
	{
		return (other.BreathType == BreathType) && (other.Stamina == Stamina) && (other.Exhaustion == Exhaustion);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtHoldingBreathSoundResult
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (Categories = "Als.Voice Variant,Als.Breath Type,Als.Holding Breath", AllowPrivateAccess))
	FGameplayTagContainer HoldingBreathType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (AllowPrivateAccess))
	float Stamina {0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (AllowPrivateAccess))
	float Exhaustion {0.0f};


	bool operator==(const FAlsxtHoldingBreathSoundResult& other) const
	{
		return (other.HoldingBreathType == HoldingBreathType) && (other.Stamina == Stamina) && (other.Exhaustion == Exhaustion);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTCharacterMovementSoundResult
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (Categories = "Als.Character Movement Sound,Als.OverlayMode", AllowPrivateAccess))
	FGameplayTagContainer Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (Categories = "Als.Object Weight", AllowPrivateAccess))
	float Weight {0.0f};

	bool operator==(const FALSXTCharacterMovementSoundResult& other) const
	{
		return (other.Type == Type) && (other.Weight == Weight);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTCharacterActionSoundResult
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (Categories = "Als.Voice Variant,Als.OverlayMode,Als.Action Strength", AllowPrivateAccess))
	FGameplayTagContainer Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (AllowPrivateAccess))
	float Stamina {0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (AllowPrivateAccess))
	float Exhaustion {0.0f};

	bool operator==(const FALSXTCharacterActionSoundResult& other) const
	{
		return (other.Type == Type) && (other.Stamina == Stamina) && (other.Exhaustion == Exhaustion);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTCharacterDamageSoundResult
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (Categories = "Als.Voice Variant,Als.Impact Form,Als.Attack Method", AllowPrivateAccess))
	FGameplayTagContainer Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (AllowPrivateAccess))
	float Damage {0.0f} ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (AllowPrivateAccess))
	float CurrentHealth {0.0f};

	bool operator==(const FALSXTCharacterDamageSoundResult& other) const
	{
		return (other.Type == Type) && (other.Damage == Damage) && (other.CurrentHealth == CurrentHealth);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTWeaponActionSoundResult
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chooser Sound Result", Meta = (Categories = "Als.Weapon,Als.Weapon Action", AllowPrivateAccess))
	FGameplayTag Type;


	bool operator==(const FALSXTWeaponActionSoundResult& other) const
	{
		return (other.Type == Type);
	}
};