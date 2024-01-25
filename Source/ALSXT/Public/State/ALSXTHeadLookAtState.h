#pragma once

#include "ALSXTHeadLookAtState.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTHeadLookAtState
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bEnableHeadLookAt{ false };

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* Actor;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector LookAtLocation{ EForceInit::ForceInit };

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRotator CalculatedLookAtHeadRotation{ EForceInit::ForceInit };
};

