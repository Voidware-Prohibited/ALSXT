#pragma once

#include "Utility/ALSXTStructs.h"
#include "Engine/DataAsset.h"
#include "Engine/EngineTypes.h"
#include "ALSXTMeshPaintingSettings.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FMeshPaintingParameters
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FGameplayTag Location{ FGameplayTag::EmptyTag };
};

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UALSXTMeshPaintingSettingsMap : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Surfaces} {PaintType} {Conditions} {FadeOut} {FadeOutDelay} {FadeOut}", AllowPrivateAccess))
	TMap<TEnumAsByte<EPhysicalSurface>, UALSXTMeshPaintingSettings*> SettingsMap;
};

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UALSXTMeshPaintingSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{Surfaces} {PaintType} {Conditions} {FadeOut} {FadeOutDelay} {FadeOut}", AllowPrivateAccess))
	TArray<FALSXTMeshPaintCriteria> MeshPaintCriteria;
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTGeneralMeshPaintingSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnableMeshPainting{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnableSaturation{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnableBloodDamage{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnableBackspatter{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnableSurfaceDamage{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnableBurnDamage{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPreserveRatio))
	FVector2D RenderTargetSize{ 1024, 1024 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnableFadeInOpacity{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnableFadeOutOpacity{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName FadeOutOpacityParameterName{ FName("Opacity") };

};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTGlobalGeneralMeshPaintingSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* FadeMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FALSXTGeneralMeshPaintingSettings GeneralSettings;
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTServerMeshPaintingSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnableFadeOutSaturation{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnableFadeOutBloodDamage{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnableFadeOutBackspatter{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnableFadeOutSurfaceDamage{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnableFadeOutBurnDamage{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FALSXTGeneralMeshPaintingSettings GeneralSettings;
};