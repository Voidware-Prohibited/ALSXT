#pragma once

#include "Engine/EngineTypes.h"
#include "ALSXTMeshRenderSettings.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTGeneralALSXTMeshRenderSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Rendering")
	bool bEnableSeperateFirstPersonFOV{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Rendering")
	bool bEnableViewModelOnSkeletalMesh{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Rendering")
	bool bEnableViewModelOnStaticMesh{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Rendering")
	bool bEnableBlendOnCharacterAction{ true };

};