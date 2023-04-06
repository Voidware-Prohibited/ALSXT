#pragma once

#include "Animation/AnimMontage.h"
#include "ALSXTFreelookSettings.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTGeneralFreelookSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnableFreelook { true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnableThirdPersonFreelook { true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnableFirstPersonFreelook { true };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FVector2D ThirdPersonYawRange {-180.0f, 180.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FVector2D ThirdPersonPitchRange {-180.0f, 180.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FVector2D FirstPersonYawRange {-180.0f, 180.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FVector2D FirstPersonPitchRange {-180.0f, 180.0f};
};
