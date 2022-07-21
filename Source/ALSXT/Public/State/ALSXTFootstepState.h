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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TWeakObjectPtr<UPhysicalMaterial> LeftFootPhysicalMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TWeakObjectPtr<UPhysicalMaterial> PrevLeftFootPhysicalMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TWeakObjectPtr<UPhysicalMaterial> RightFootPhysicalMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TWeakObjectPtr<UPhysicalMaterial> PrevRightFootPhysicalMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 1))
		float LeftFootWetness{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 1))
		float RightFootWetness{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 1))
		float LeftFootBlood{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 1))
		float RightFootBlood{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 1))
		float LeftFootDirt1{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 1))
		float RightFootDirt1{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 1))
		float LeftFootDirt2{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 1))
		float RightFootDirt2{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 1))
		float LeftFootSand{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 1))
		float RightFootSand{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 1))
		float LeftFootVegetation{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 1))
		float RightFootVegetation{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 1))
		float LeftFootSnow{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 1))
		float RightFootSnow{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 1))
		float LeftFootOil{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 1))
		float RightFootOil{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float LeftFootDurationModifierMin{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RightFootDurationModifierMin{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float LeftFootDurationModifierMax{ 4.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RightFootDurationModifierMax{ 4.0f };
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTFootwearDetails
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* FootwearSoleTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* FootwearSoleNormal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TWeakObjectPtr<UPhysicalMaterial> FootwearSoleSurfaceType;

};