#pragma once

#include "ALSXT/Public/Settings/AlsxtStationaryModeSettings.h"
#include "ALS/Public/Settings/AlsFeetSettings.h"
#include "ALS/Public/Settings/AlsGeneralAnimationSettings.h"
#include "ALS/Public/Settings/AlsGroundedSettings.h"
#include "ALS/Public/Settings/AlsInAirSettings.h"
#include "ALS/Public/Settings/AlsRotateInPlaceSettings.h"
#include "ALS/Public/Settings/AlsTransitionsSettings.h"
#include "ALS/Public/Settings/AlsTurnInPlaceSettings.h"
#include "ALS/Public/Settings/AlsHeadSettings.h"
#include "Engine/DataAsset.h"
#include "Settings/AlsStandingSettings.h"
#include "AlsxtAnimationInstanceSettings.generated.h"

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UAlsxtAnimationInstanceSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UMirrorDataTable> MirrorTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FAlsStandingSettings Crouching;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FAlsxtStationaryModeAnimations StationaryMode;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FAlsGeneralAnimationSettings General;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FAlsHeadSettings Head;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FAlsGroundedSettings Grounded;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FAlsInAirSettings InAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FAlsFeetSettings Feet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FAlsTransitionsSettings Transitions;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FAlsRotateInPlaceSettings RotateInPlace;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FAlsGeneralTurnInPlaceSettings TurnInPlace;

public:
	UAlsxtAnimationInstanceSettings();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
