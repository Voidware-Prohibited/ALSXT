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

};