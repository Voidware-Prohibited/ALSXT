#pragma once

#include "State/AlsViewState.h"
#include "ALSXTHeadLookAtState.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTHeadLookAtState
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRotator LockedControlRotation{ EForceInit::ForceInit };

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector2D LockedLookInput;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FAlsViewState LockedViewState;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRotator FreeLookHeadRotation{ EForceInit::ForceInit };

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bEnableHeadLookAt{ false };

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* HeadLookAtActor{ nullptr };

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector HeadLookAtLocation{ EForceInit::ForceInit };

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRotator HeadLookAtRotation{ EForceInit::ForceInit };
};

