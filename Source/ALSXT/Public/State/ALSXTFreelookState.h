#pragma once

#include "GameplayTagContainer.h"
#include "State/AlsViewState.h"
#include "ALSXTFreelookState.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTFreelookState
{
	GENERATED_BODY()
		
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FGameplayTag Freelooking;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector2D LockedLookInput { EForceInit::ForceInit };

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FAlsViewState LockedViewState;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = -180, ClampMax = 180, ForceUnits = "deg"))
		FRotator LockedControlRotation { EForceInit::ForceInit };

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRotator FreeLookHeadRotation { EForceInit::ForceInit };

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = -180, ClampMax = 180, ForceUnits = "deg"))
		float LockedYawAngle { 0.0f };

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = -180, ClampMax = 180, ForceUnits = "deg"))
		float LockedPitchAngle { 0.0f };

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ClampMin = -180, ClampMax = 180, ForceUnits = "deg"))
		float FreelookYawAngle{ 0.0f };

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ClampMin = -90, ClampMax = 90, ForceUnits = "deg"))
		float FreelookPitchAngle{ 0.0f };
};

