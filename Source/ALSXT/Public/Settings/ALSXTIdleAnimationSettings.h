#pragma once

#include "Utility/ALSXTStructs.h"
#include "ALSXTIdleAnimationSettings.generated.h"

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UALSXTIdleAnimationSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations", Meta = (TitleProperty = "{Health} {Montage}", AllowPrivateAccess))
	TArray<FIdleAnimation> IdleAnimations;

};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTALSXTGeneralIdleAnimationSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool EnableIdleAnimations{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool EnableIdleCameraRotation{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0))
	float IdleDurationTrigger{ 4.00f };
};