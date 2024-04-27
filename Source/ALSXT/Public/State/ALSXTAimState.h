#pragma once

#include "ALSXTAimState.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTAimState
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = -180, ClampMax = 180, ForceUnits = "deg"))
		FRotator ControlRotation { EForceInit::ForceInit };

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector OriginPoint{ EForceInit::ForceInit };

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTransform OriginTransform{ ENoInit::NoInit };

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector TargetPoint{ EForceInit::ForceInit };

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTransform TargetTransform{ ENoInit::NoInit };

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTransform OpticTransform{ ENoInit::NoInit };

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTransform FrontSightTransform{ ENoInit::NoInit };

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector BarrelPoint{ EForceInit::ForceInit };
};

