#pragma once

#include "Utility/AlsMath.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Engine/Texture2D.h"
#include "Engine/EngineTypes.h"
#include "ALSXTFootstepState.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTFootstepState
{
	GENERATED_BODY()

	UPROPERTY(Meta = (AllowPrivateAccess, ClampMin = 0))
	TWeakObjectPtr<UPhysicalMaterial> LeftFootPhysicalMaterial;

	UPROPERTY(Meta = (AllowPrivateAccess, ClampMin = 0))
	TWeakObjectPtr<UPhysicalMaterial> PrevLeftFootPhysicalMaterial;

	UPROPERTY(Meta = (AllowPrivateAccess, ClampMin = 0))
	TWeakObjectPtr<UPhysicalMaterial> RightFootPhysicalMaterial;

	UPROPERTY(Meta = (AllowPrivateAccess, ClampMin = 0))
	TWeakObjectPtr<UPhysicalMaterial> PrevRightFootPhysicalMaterial;

	UPROPERTY(Meta = (AllowPrivateAccess, ClampMin = 0))
	UTexture2D* LeftFootSoleTexture;
	
	UPROPERTY(Meta = (AllowPrivateAccess, ClampMin = 0))
	UTexture2D* RightFootSoleTexture;
	
	UPROPERTY(Meta = (AllowPrivateAccess, ClampMin = 0))
	UTexture2D* LeftFootSoleNormal;
	
	UPROPERTY(Meta = (AllowPrivateAccess, ClampMin = 0))
	UTexture2D* RightFootSoleNormal;
	
	UPROPERTY(Meta = (AllowPrivateAccess, ClampMin = 0))
	UTexture2D* LeftFootSoleDetail;
	
	UPROPERTY(Meta = (AllowPrivateAccess, ClampMin = 0))
	UTexture2D* RightFootSoleDetail;
	
	UPROPERTY(Meta = (AllowPrivateAccess, ClampMin = 0))
	float LeftFootSoleDetailScale{ 0.0f };
	
	UPROPERTY(Meta = (AllowPrivateAccess, ClampMin = 0))
	float RightFootSoleDetailScale{ 0.0f };
	
	UPROPERTY(Meta = (AllowPrivateAccess, ClampMin = 0))
	FLinearColor LeftFootMaterialPrimaryColor{0.0f, 0.0f, 0.0f, 1.0f};
	
	UPROPERTY(Meta = (AllowPrivateAccess, ClampMin = 0))
	FLinearColor RightFootMaterialPrimaryColor{0.0f, 0.0f, 0.0f, 1.0f};
	
	UPROPERTY(Meta = (AllowPrivateAccess, ClampMin = 0))
	FLinearColor LeftFootMaterialSecondaryColor{0.0f, 0.0f, 0.0f, 1.0f};
	
	UPROPERTY(Meta = (AllowPrivateAccess, ClampMin = 0))
	FLinearColor RightFootMaterialSecondaryColor{0.0f, 0.0f, 0.0f, 1.0f};
	
	UPROPERTY(Meta = (AllowPrivateAccess, ClampMin = 0))
	float LeftFootMaterialGrainSize{ 1.0f };
	
	UPROPERTY(Meta = (AllowPrivateAccess, ClampMin = 0))
	float RightFootMaterialGrainSize{ 1.0f };
	
	UPROPERTY(Meta = (AllowPrivateAccess, ClampMin = 0))
	float LeftFootMaterialWetness{ 0.0f };
	
	UPROPERTY(Meta = (AllowPrivateAccess, ClampMin = 0))
	float RightFootMaterialWetness{ 0.0f };
	
	UPROPERTY(Meta = (AllowPrivateAccess, ClampMin = 0))
	float LeftFootMaterialTransferAmount{ 0.5f };
	
	UPROPERTY(Meta = (AllowPrivateAccess, ClampMin = 0))
	float RightFootMaterialTransferAmount{ 0.5f };
	
	UPROPERTY(Meta = (AllowPrivateAccess, ClampMin = -1))
	float LeftFootDecalNormalAmount{0.0f};
	
	UPROPERTY(Meta = (AllowPrivateAccess, ClampMin = -1))
	float RightFootDecalNormalAmount{0.0f};

	UPROPERTY(Meta = (AllowPrivateAccess, ClampMin = 0))
	float LeftFootDecalEmissiveAmount{ 0.0f };

	UPROPERTY(Meta = (AllowPrivateAccess, ClampMin = 0))
	float RightFootDecalEmissiveAmount{ 0.0f };
	
	UPROPERTY(Meta = (AllowPrivateAccess, ClampMin = 0))
	float LeftFootDecalDuration{4.0f};
	
	UPROPERTY(Meta = (AllowPrivateAccess, ClampMin = 0))
	float RightFootDecalDuration{4.0f};
	
	UPROPERTY(Meta = (AllowPrivateAccess, ClampMin = 0))
	float LeftFootDecalFadeOutDuration{2.0f};
	
	UPROPERTY(Meta = (AllowPrivateAccess, ClampMin = 0))
	float RightFootDecalFadeOutDuration{2.0f};

	UPROPERTY(Meta = (AllowPrivateAccess, ClampMin = 0))
	float LeftFootDurationModifierMin{ 1.0f };

	UPROPERTY(Meta = (AllowPrivateAccess, ClampMin = 0))
	float RightFootDurationModifierMin{ 1.0f };

	UPROPERTY(Meta = (AllowPrivateAccess, ClampMin = 0))
	float LeftFootDurationModifierMax{ 4.0f };

	UPROPERTY(Meta = (AllowPrivateAccess, ClampMin = 0))
	float RightFootDurationModifierMax{ 4.0f };
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTFootwearDetails
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
		UTexture2D* FootwearSoleTexture;

	UPROPERTY(BlueprintReadWrite)
		UTexture2D* FootwearSoleNormal;

	UPROPERTY(BlueprintReadWrite)
		TWeakObjectPtr<UPhysicalMaterial> FootwearSoleSurfaceType;

};