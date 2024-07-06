#pragma once

#include "State/AlsViewState.h"
#include "ALSXTHeadLookAtState.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTHeadLookAtState
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bEnableHeadLookAt{ false };

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* HeadLookAtActor{ nullptr };

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector HeadLookAtLocation{ EForceInit::ForceInit };

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bEnableCameraOffset{ false };

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector CameraOffset{ EForceInit::ForceInit };

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRotator HeadLookAtRotation{ EForceInit::ForceInit };
};

