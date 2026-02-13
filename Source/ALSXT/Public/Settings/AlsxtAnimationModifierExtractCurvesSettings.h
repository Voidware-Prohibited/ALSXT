// Copyright (C) 2026 Uriel Ballinas, VOIDWARE Prohibited. All rights reserved.
// This software is licensed under the MIT License (LICENSE.md).

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AlsxtAnimationModifierExtractCurvesSettings.generated.h"

UCLASS(config = PluginSetting)
class ALSXT_API UAlsxtAnimationModifierExtractCurvesSettings : public UObject
{
	GENERATED_BODY()

public:
	UAlsxtAnimationModifierExtractCurvesSettings(const FObjectInitializer& Obj);

	/** Default location for curves */
	UPROPERTY(Config, EditAnywhere, Category = "Data Saving")
	FString CurvesSavePath{"/Game/Curves/ProceduralRecoil/"};
};