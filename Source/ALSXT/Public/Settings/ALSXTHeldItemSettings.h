#pragma once

#include "Utility/ALSXTStructs.h"
#include "ALSXTHeldItemSettings.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTHeldItemSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag Overlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer AvailableCombatStances;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer AvailableWeaponReadyPositions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag DefaultWeaponReadyPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer AvailableWeaponCarryPositions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer AvailableGripPositions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	bool UseLeftHandIK {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
	FName AttackTraceStartSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
	FName AttackTraceEndSocket;

};

USTRUCT(BlueprintType)
struct ALSXT_API FHeldItemMontage
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	TObjectPtr<UAnimMontage> Montage { nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	TObjectPtr<UCurveFloat> BlendInCurve { nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (ClampMin = 0))
	FVector2D StartTime{0.5f, 0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (ClampMin = 0))
	FVector2D PlayRate{1.0f, 1.0f};

	bool operator==(const FHeldItemMontage& other) const
	{
		return (other.Montage == Montage) && (other.BlendInCurve == BlendInCurve) && (other.StartTime == StartTime) && (other.PlayRate == PlayRate);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FHeldItemEquipMontage
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
	FGameplayTag Type{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
	FGameplayTag WeaponCarryPosition{FGameplayTag::EmptyTag};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	FHeldItemMontage Montage;

	bool operator==(const FHeldItemEquipMontage& other) const
	{
		return (other.Montage == Montage);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FHeldItemActionMontage
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
	FGameplayTag Type{FGameplayTag::EmptyTag};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	FHeldItemMontage Montage;

	bool operator==(const FHeldItemActionMontage& other) const
	{
		return (other.Montage == Montage);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FHeldItemAttackMontage
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
	FGameplayTag Type{ FGameplayTag::EmptyTag };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
	FGameplayTag Strength{ FGameplayTag::EmptyTag };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
	float BaseStaminaCost{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	FHeldItemMontage Montage;

	bool operator==(const FHeldItemAttackMontage& other) const
	{
		return (other.Montage == Montage);
	}
};