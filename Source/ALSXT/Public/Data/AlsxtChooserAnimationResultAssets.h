#pragma once

#include "Engine/DataAsset.h"
#include "Utility/AlsxtAnimationStructs.h"
#include "Utility/AlsxtOverlayStructs.h"

#include "AlsxtChooserAnimationResultAssets.generated.h"

struct FAlsxtLocomotionAnimationInstanceAssets;
struct FAlsxtOverlayAnimationInstanceAssets;

UCLASS(Blueprintable, BlueprintType)
class UAlsxtChooserAnimationLocomotionResultAsset: public UDataAsset
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ShowOnlyInnerProperties, AllowPrivateAccess))
	FAlsxtLocomotionAnimationInstanceAssets Assets;
	
};

UCLASS(Blueprintable, BlueprintType)
class UAlsxtChooserAnimationOverlayResultAsset: public UDataAsset
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ShowOnlyInnerProperties, AllowPrivateAccess))
	FAlsxtOverlayAnimationInstanceAssets Assets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (Categories = "Als.OverlaySlot", AllowPrivateAccess))
	FGameplayTagContainer AvailableSlots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (Categories = "Als.Gait", AllowPrivateAccess))
	FGameplayTagContainer AvailableGaits;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (Categories = "Als.Stance", AllowPrivateAccess))
	FGameplayTagContainer AvailableStances;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (Categories = "Als.Ready Stance", AllowPrivateAccess))
	FGameplayTagContainer AvailableReadyStances;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (Categories = "Als.OverlaySettings", AllowPrivateAccess))
	FGameplayTagContainer Settings;
	
};

UCLASS(Blueprintable, BlueprintType)
class UAlsxtChooserAnimationAdditiveIdleResultAsset: public UDataAsset
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ShowOnlyInnerProperties, AllowPrivateAccess))
	TSubclassOf<UAnimSequenceBase> Asset;
	
};