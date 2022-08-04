#pragma once

#include "Utility/AlsMath.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Engine/Texture2D.h"
#include "Engine/EngineTypes.h"
#include "ALSXTFootstepState.generated.h"

UENUM(BlueprintType)
enum class EALSXTFootBone : uint8
{
	Left,
	Right,
};

UENUM(BlueprintType)
enum class EALSXTFootprintPhase : uint8
{
	Current,
	Previous,
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTFootprintStatePhase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TEnumAsByte<EPhysicalSurface> SurfaceType {0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	UTexture2D* TransferTexture = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	UTexture2D* TransferNormalTexture = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	UTexture2D* TransferDetailTexture = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	UTexture2D* TransferDetailNormal = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float SurfaceTransferAmount{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float SurfaceTransferAcceptanceAmount{ 0.5f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = -2))
	float SurfaceTransferAcceptanceNormalScale{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float TransferAmount{ 0.5f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float TransferSaturationRate{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float TransferDesaturationRate{ 1.0f };
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float TransferDetailScale{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float TransferDetailNormalAmount{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float TransferNormalScale{ 0.5f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FLinearColor TransferPrimaryColor{0.0f, 0.0f, 0.0f, 1.0f};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FLinearColor TransferSecondaryColor{0.0f, 0.0f, 0.0f, 1.0f};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float TransferGrainSize{ 1.0f };
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float TransferWetness{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float TransferEmissiveAmount{ 0.0f };
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float DecalDuration{4.0f};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float DecalFadeOutDuration{2.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float DecalDurationModifierMin{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float DecalDurationModifierMax{ 4.0f };
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTFootprintState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float FootSurfaceAlpha{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FALSXTFootprintStatePhase Current;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FALSXTFootprintStatePhase Previous;

};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTFootprintsState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FALSXTFootprintState Left;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FALSXTFootprintState Right;

};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTFootwearDetails
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* FootwearSoleTexture = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* FootwearSoleNormalTexture = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FootwearSoleNormalScale{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* FootwearSoleDetailTexture = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EPhysicalSurface> FootwearSoleSurfaceType {0};

};