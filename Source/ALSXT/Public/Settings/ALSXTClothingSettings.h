#pragma once

#include "Utility/ALSXTStructs.h"
#include "Engine/DataAsset.h"
#include "Engine/EngineTypes.h"
#include "ALSXTClothingSettings.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTClothingItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FGameplayTag ClothingSlot{FGameplayTag::EmptyTag};
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FGameplayTagContainer ClothingState{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	TEnumAsByte<EPhysicalSurface> SurfaceType {SurfaceType_Default};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0))
	float BaseDamage{ 0.0f };
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTClothingParameters
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	TArray<FALSXTClothingItem> ClothingItems;
};

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UALSXTClothingSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing", Meta = (AllowPrivateAccess))
	bool bBool{ true };
};


USTRUCT(BlueprintType)
struct ALSXT_API FALSXTGeneralClothingSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Features", Meta = (AllowPrivateAccess))
	bool bEnableClothing{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Features", Meta = (AllowPrivateAccess))
	bool bEnableAccessories{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Features", Meta = (AllowPrivateAccess))
	bool bEnableMedical{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Features", Meta = (AllowPrivateAccess))
	bool bEnableHeadwear{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Features", Meta = (AllowPrivateAccess))
	bool bEnableFacewear{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Features", Meta = (AllowPrivateAccess))
	bool bEnableEyewear{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Features", Meta = (AllowPrivateAccess))
	bool bEnableEarwear{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Features", Meta = (AllowPrivateAccess))
	bool bEnableTop{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Features", Meta = (AllowPrivateAccess))
	bool bEnableTopUnderwear{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Features", Meta = (AllowPrivateAccess))
	bool bEnableVest{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Features", Meta = (AllowPrivateAccess))
	bool bEnableJacket{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Features", Meta = (AllowPrivateAccess))
	bool bEnableBottom{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Features", Meta = (AllowPrivateAccess))
	bool bEnableBottomUnderwear{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Features", Meta = (AllowPrivateAccess))
	bool bEnableBelt{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Features", Meta = (AllowPrivateAccess))
	bool bEnableLeftWrist{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Features", Meta = (AllowPrivateAccess))
	bool bEnableRightWrist{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Features", Meta = (AllowPrivateAccess))
	bool bEnableGloves{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Features", Meta = (AllowPrivateAccess))
	bool bFeetwear{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Features", Meta = (AllowPrivateAccess))
	bool bEnableBackpack{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Features", Meta = (AllowPrivateAccess))
	bool bEnableNecklace{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Features", Meta = (AllowPrivateAccess))
	bool bEnableEarrings{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Features", Meta = (AllowPrivateAccess))
	bool bEnableBodyMod01{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Features", Meta = (AllowPrivateAccess))
	bool bEnableBodyMod02{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Features", Meta = (AllowPrivateAccess))
	bool bEnableBodyMod03{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Features", Meta = (AllowPrivateAccess))
	bool bEnableMedical01{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Features", Meta = (AllowPrivateAccess))
	bool bEnableMedical02{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Features", Meta = (AllowPrivateAccess))
	bool bEnableMedical03{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Features", Meta = (AllowPrivateAccess))
	bool bEnableMedical04{ true };

};