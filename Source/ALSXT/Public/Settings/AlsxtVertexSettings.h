#pragma once

#include "Engine/DataAsset.h"
#include "Utility/AlsxtVertexEnums.h"
#include "AlsxtVertexSettings.generated.h"

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UAlsxtVertexColorPhysicalMaterialMap : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ForceInlineRow, AllowPrivateAccess))
	TMap<EVertexColorChannel, UPhysicalMaterial*> VertexColorPhysicalMaterialMap{
			{EVertexColorChannel::Red, {}},
			{EVertexColorChannel::Green, {}},
			{EVertexColorChannel::Blue, {}},
			{EVertexColorChannel::White, {}},
			{EVertexColorChannel::Alpha, {}}
	};
};
