#pragma once

#include "Animation/AnimMontage.h"
#include "ALSXTSlidingSettings.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTSlidingSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimMontage> Montage{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0))
		float MinimumSpeedToSlide{ 350.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0))
		float MaximumSlidingDuration{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0))
		float AutoSlideOnSlopeDegree{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bCrouchOnStart{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bRotateToInputOnStart{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0))
		float RotationInterpolationSpeed{ 10.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bInterruptSlidingWhenInAir{ true };
};
