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
struct ALSXT_API FALSXTFootstepState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TWeakObjectPtr<UPhysicalMaterial> LeftFootPhysicalMaterial = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TWeakObjectPtr<UPhysicalMaterial> PrevLeftFootPhysicalMaterial = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TWeakObjectPtr<UPhysicalMaterial> RightFootPhysicalMaterial = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TWeakObjectPtr<UPhysicalMaterial> PrevRightFootPhysicalMaterial = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	UTexture2D* LeftFootSoleTexture = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	UTexture2D* RightFootSoleTexture = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	UTexture2D* LeftFootSoleNormal = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	UTexture2D* RightFootSoleNormal = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	UTexture2D* LeftFootSoleDetail = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	UTexture2D* RightFootSoleDetail = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float LeftFootSoleDetailScale{ 0.0f };
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float RightFootSoleDetailScale{ 0.0f };
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FLinearColor LeftFootMaterialPrimaryColor{0.0f, 0.0f, 0.0f, 1.0f};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FLinearColor RightFootMaterialPrimaryColor{0.0f, 0.0f, 0.0f, 1.0f};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FLinearColor LeftFootMaterialSecondaryColor{0.0f, 0.0f, 0.0f, 1.0f};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FLinearColor RightFootMaterialSecondaryColor{0.0f, 0.0f, 0.0f, 1.0f};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float LeftFootMaterialGrainSize{ 1.0f };
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float RightFootMaterialGrainSize{ 1.0f };
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float LeftFootMaterialWetness{ 0.0f };
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float RightFootMaterialWetness{ 0.0f };
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float LeftFootMaterialTransferAmount{ 0.5f };
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float RightFootMaterialTransferAmount{ 0.5f };
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float LeftFootDecalNormalAmount{0.0f};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float RightFootDecalNormalAmount{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float LeftFootDecalEmissiveAmount{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float RightFootDecalEmissiveAmount{ 0.0f };
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float LeftFootDecalDuration{4.0f};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float RightFootDecalDuration{4.0f};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float LeftFootDecalFadeOutDuration{2.0f};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float RightFootDecalFadeOutDuration{2.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float LeftFootDurationModifierMin{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float RightFootDurationModifierMin{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float LeftFootDurationModifierMax{ 4.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float RightFootDurationModifierMax{ 4.0f };
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTFootwearDetails
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* FootwearSoleTexture = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* FootwearSoleNormal = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TWeakObjectPtr<UPhysicalMaterial> FootwearSoleSurfaceType;

};