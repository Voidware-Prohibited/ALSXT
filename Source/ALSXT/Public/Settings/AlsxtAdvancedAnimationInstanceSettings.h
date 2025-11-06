#pragma once

#include "ALS/Public/Settings/AlsFeetSettings.h"
#include "ALS/Public/Settings/AlsGeneralAnimationSettings.h"
#include "ALS/Public/Settings/AlsGroundedSettings.h"
#include "ALS/Public/Settings/AlsInAirSettings.h"
#include "ALS/Public/Settings/AlsRotateInPlaceSettings.h"
#include "ALS/Public/Settings/AlsTransitionsSettings.h"
#include "ALS/Public/Settings/AlsTurnInPlaceSettings.h"
#include "ALS/Public/Settings/AlsHeadSettings.h"
#include "Engine/DataAsset.h"
#include "AlsxtAdvancedAnimationInstanceSettings.generated.h"

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UAlsxtAdvancedAnimationInstanceSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	bool Param;

public:
	UAlsxtAdvancedAnimationInstanceSettings();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
