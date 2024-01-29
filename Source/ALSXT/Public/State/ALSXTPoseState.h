#pragma once

#include "ALSXTPoseState.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTPoseState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ClampMin = 0, ClampMax = 1))
	float LeanLeftAmount{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ClampMin = 0, ClampMax = 1))
	float LeanRightAmount{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ClampMin = 0, ClampMax = 1))
	float CantedAmount{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ClampMin = 0, ClampMax = 1))
	float VaultingAmount{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ClampMin = 0, ClampMax = 1))
	float SlidingAmount{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ClampMin = 0, ClampMax = 1))
	float WallJumpAmount{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ClampMin = 0, ClampMax = 1))
	float WallRunAmount{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ClampMin = 0, ClampMax = 1))
	float StationaryAmount{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ClampMin = 0, ClampMax = 1))
	float SeatedAmount{0.0f};
};