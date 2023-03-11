#pragma once

#include "Utility/ALSXTStructs.h"
#include "Engine/DataAsset.h"
#include "Engine/EngineTypes.h"
#include "ALSXTBloodSpatterSettings.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTBloodSpatterParameters
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FGameplayTag Location{FGameplayTag::EmptyTag};
};

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UALSXTBloodSpatterSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Velocity} {Volume} {Distance} {MaxDripSpeed}", AllowPrivateAccess))
	TArray<FALSXTBloodSpatterType> ActionSounds;
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTGeneralBloodSpatterSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAllowBloodSpatters{ true };

};