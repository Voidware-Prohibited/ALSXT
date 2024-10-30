#pragma once

#include "Engine/EngineTypes.h"
#include "Utility/ALSXTAnimationStructs.h"
#include "ALSXTDefensiveModeSettings.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTDefensiveModeSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	TArray<TEnumAsByte<EObjectTypeQuery>> AnticipationTraceObjectTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	TArray<TEnumAsByte<EObjectTypeQuery>> FallingAnticipationTraceObjectTypes;
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTDefensivePoseStanceSet
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	FPoseSingleFrame Front;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	FPoseSingleFrame Back;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	FPoseSingleFrame Left;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	FPoseSingleFrame Right;

	bool operator==(const FALSXTDefensivePoseStanceSet& other) const
	{
		return (other.Front == Front) && (other.Back == Back) && (other.Left == Left) && (other.Right == Right);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTAnticipationMovementModePoseSet
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	FALSXTDefensivePoseStanceSet High;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	FALSXTDefensivePoseStanceSet Middle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	FALSXTDefensivePoseStanceSet Low;

	bool operator==(const FALSXTAnticipationMovementModePoseSet& other) const
	{
		return (other.High == High) && (other.Middle == Middle) && (other.Low == Low);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTObstacleCollisionAnticipationPoseSet
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	FALSXTDefensivePoseStanceSet Standing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	FALSXTDefensivePoseStanceSet Crouched;

	bool operator==(const FALSXTObstacleCollisionAnticipationPoseSet& other) const
	{
		return (other.Standing == Standing) && (other.Crouched == Crouched);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTDefensivePoseSet
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	FALSXTAnticipationMovementModePoseSet Standing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	FALSXTDefensivePoseStanceSet Crouched;

	bool operator==(const FALSXTDefensivePoseSet& other) const
	{
		return (other.Standing == Standing) && (other.Crouched == Crouched);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTAnticipationPoseSet
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	FALSXTDefensivePoseSet Grounded;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	FALSXTAnticipationMovementModePoseSet InAir;

	bool operator==(const FALSXTAnticipationPoseSet& other) const
	{
		return (other.Grounded == Grounded) && (other.InAir == InAir);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FAnticipationPoses
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.OverlayMode", AllowPrivateAccess))
	FGameplayTagContainer Overlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Impact Form", AllowPrivateAccess))
	FGameplayTagContainer Form;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Locomotion Variant", AllowPrivateAccess))
	FGameplayTagContainer Variant;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Standing} {Crouched} {InAir}", AllowPrivateAccess))
	FALSXTAnticipationPoseSet Poses;

	bool operator==(const FAnticipationPoses& other) const
	{
		return (other.Overlay == Overlay) && (other.Form == Form) && (other.Variant == Variant) && (other.Poses == Poses);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FAvoidingPoses
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.OverlayMode", AllowPrivateAccess))
	FGameplayTagContainer Overlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Impact Form", AllowPrivateAccess))
	FGameplayTagContainer Form;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Locomotion Variant", AllowPrivateAccess))
	FGameplayTagContainer Variant;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Standing} {Crouched}", AllowPrivateAccess))
	FALSXTDefensivePoseSet Poses;

	bool operator==(const FAvoidingPoses& other) const
	{
		return (other.Overlay == Overlay) && (other.Form == Form) && (other.Variant == Variant) && (other.Poses == Poses);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FBraceForImpactPoses
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.OverlayMode", AllowPrivateAccess))
	FGameplayTagContainer Overlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Impact Form", AllowPrivateAccess))
	FGameplayTagContainer Form;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Locomotion Variant", AllowPrivateAccess))
	FGameplayTagContainer Variant;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{High} {Middle} {Low}", AllowPrivateAccess))
	FALSXTDefensivePoseSet Poses;

	bool operator==(const FBraceForImpactPoses& other) const
	{
		return (other.Overlay == Overlay) && (other.Form == Form) && (other.Variant == Variant) && (other.Poses == Poses);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FObstacleCollisionAnticipationPoses
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.OverlayMode", AllowPrivateAccess))
	FGameplayTagContainer Overlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Impact Form", AllowPrivateAccess))
	FGameplayTagContainer Form;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Locomotion Variant", AllowPrivateAccess))
	FGameplayTagContainer Variant;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{High} {Middle} {Low}", AllowPrivateAccess))
	FALSXTObstacleCollisionAnticipationPoseSet Poses;

	bool operator==(const FObstacleCollisionAnticipationPoses& other) const
	{
		return (other.Overlay == Overlay) && (other.Form == Form) && (other.Variant == Variant) && (other.Poses == Poses);
	}
};