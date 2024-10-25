#pragma once

#include "NativeGameplayTags.h"
#include "Settings/ALSXTDefensiveModeSettings.h"
#include "ALSXTCollisionStructs.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FCrowdNavigationPoses
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.OverlayMode", AllowPrivateAccess))
	FGameplayTagContainer Overlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Front}, {Back}, {Left}, {Right}", AllowPrivateAccess))
	FALSXTDefensivePoseStanceSet Poses;

	bool operator==(const FCrowdNavigationPoses& other) const
	{
		return (other.Overlay == Overlay) && (other.Poses == Poses);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FObstaclePose
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.OverlayMode", AllowPrivateAccess))
	FGameplayTagContainer Overlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Game.Health", AllowPrivateAccess))
	FGameplayTagContainer Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FALSXTDefensivePoseSet Poses;

	bool operator==(const FObstaclePose& other) const
	{
		return (other.Overlay == Overlay) && (other.Health == Health) && (other.Poses == Poses);
	}
};