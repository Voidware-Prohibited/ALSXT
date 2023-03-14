#pragma once

#include "Animation/AnimMontage.h"
#include "ALSXTSlidingSettings.generated.h"

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UALSXTSlidingSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TArray<TObjectPtr<UAnimMontage>> RegularMontages;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TArray<TObjectPtr<UAnimMontage>> AngleMontages;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TArray<TObjectPtr<UAnimMontage>> AngleUncontrolledMontages;
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTGeneralSlidingSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAllowSliding { true };

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
