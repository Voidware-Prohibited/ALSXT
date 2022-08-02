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
	TEnumAsByte<EPhysicalSurface> FootPhysicalMaterial {0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	UTexture2D* FootSoleTexture = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	UTexture2D* FootSoleNormal = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	UTexture2D* FootTransferDetailTexture = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	UTexture2D* FootTransferDetailNormal = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float FootTransferAmount{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float FootSurfaceTransferAcceptanceAmount{ 0.5f };
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float FootTransferDetailScale{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FLinearColor FootMaterialPrimaryColor{0.0f, 0.0f, 0.0f, 1.0f};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FLinearColor FootMaterialSecondaryColor{0.0f, 0.0f, 0.0f, 1.0f};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float FootMaterialGrainSize{ 1.0f };
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float FootMaterialWetness{ 0.0f };
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float FootMaterialTransferAmount{ 0.5f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float FootMaterialSaturationRate{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float FootMaterialDesaturationRate{ 1.0f };
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float FootDecalNormalAmount{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float FootMaterialTransferDetailNormalAmount{ 0.5f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float FootDecalEmissiveAmount{ 0.0f };
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float FootDecalDuration{4.0f};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float FootDecalFadeOutDuration{2.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float FootDurationModifierMin{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float FootDurationModifierMax{ 4.0f };
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
		TWeakObjectPtr<UPhysicalMaterial> FootwearSoleSurfaceType;

};