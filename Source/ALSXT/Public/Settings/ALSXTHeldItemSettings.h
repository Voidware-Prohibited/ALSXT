#pragma once

#include "Utility/ALSXTStructs.h"
#include "Animation/AnimSequence.h"
#include "ALSXTHeldItemSettings.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTGeneralHeldItemSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm", Meta = (AllowPrivateAccess))
	FName GripSocketName{ "Grip" };

};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTItemSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag Type {FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Mass{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	bool Physics{ false };
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTHeldItemSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FALSXTItemSettings ItemSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag Overlay {FGameplayTag::EmptyTag};

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
	bool UsesLeftHandIK {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	bool Aimable{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	bool UsesRecoil{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	bool CanAttack{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	bool PhysicsWhenHeld{ false };
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
struct ALSXT_API FALSXTHeldItemFingerAnimation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Format")
	FGameplayTag Animation{ FGameplayTag::EmptyTag };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm", Meta = (AllowPrivateAccess))
	TObjectPtr<UAnimMontage> Montage;

	bool operator==(const FALSXTHeldItemFingerAnimation& other) const
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
	FName AttackTraceStartSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
	FName AttackTraceEndSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	FHeldItemMontage Montage;

	bool operator==(const FHeldItemAttackMontage& other) const
	{
		return (other.Montage == Montage);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FHeldItemGrip
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
	FGameplayTag Type{ FGameplayTag::EmptyTag };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	TObjectPtr<UAnimSequence> Pose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
	FName HandSocketName{ "Grip" };

	bool operator==(const FHeldItemGrip& other) const
	{
		return (other.Pose == Pose);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTHeldItemGripState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FHeldItemGrip Grip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attachment")
	FName AttachmentSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attachment")
	FTransform GripTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parent")
	FGameplayTag GripPosition{ FGameplayTag::EmptyTag };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Foregrip Position", TitleProperty = "{Position}", AllowPrivateAccess), Category = "Parent")
	FGameplayTag ComponentPosition;

	bool operator==(const FALSXTHeldItemGripState& other) const
	{
		return (other.Grip == Grip) && (other.AttachmentSocketName == AttachmentSocketName);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTHeldItemGripStates
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FALSXTHeldItemGripState Grip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FALSXTHeldItemGripState Foregrip;

	bool operator==(const FALSXTHeldItemGripStates& other) const
	{
		return (other.Grip == Grip) && (other.Foregrip == Foregrip);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTHeldItemState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FALSXTHeldItemGripStates GripState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag FingerPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag FingerAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag WeaponAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag WeaponAttack;

	bool operator==(const FALSXTHeldItemState& other) const
	{
		return (other.GripState == GripState) && (other.FingerPosition == FingerPosition) && (other.FingerAction == FingerAction) && (other.WeaponAction == WeaponAction) && (other.WeaponAttack == WeaponAttack);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTHeldItemViewTarget
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform Transform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FOV;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float VignetteIntensity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Attachment;

	bool operator==(const FALSXTHeldItemViewTarget& other) const
	{
		return (other.Transform.Equals(Transform, 9.99)) && (other.FOV == FOV) && (other.VignetteIntensity == VignetteIntensity) && (other.Attachment == Attachment);
	}
};