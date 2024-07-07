#pragma once

#include "ALSXTBreathState.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTTargetBreathState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Holding Breath"))
	FGameplayTag HoldingBreath{ ALSXTHoldingBreathTags::False };
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 2))
	float Rate{ 0.5 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 1))
	float Alpha{ 0.33 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 1))
	int ReleaseBreaths{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 1))
	float ReleaseStartRate{ 0.33f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 1))
	int RecoveryBreaths{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 100))
	float TransitionRate{ 1 };
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTBreathState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Breath Type"))
	FGameplayTag BreathType{ ALSXTBreathTypeTags::Regular };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Holding Breath"))
	FGameplayTag HoldingBreath{ ALSXTHoldingBreathTags::False };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 3))
	float MaxHoldBreathTime{ 2.0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 3))
	float CurrentMaxHoldBreathTime{ 2.0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 3))
	float CurrentHoldBreathTime{ 2.0 };
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 1))
	float CurrentBreathRate { 1.0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 1))
	float PreviousBreathRate{ 1.0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 1))
	float CurrentBreathAlpha { 0.75 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 1))
	float PreviousBreathAlpha{ 0.75 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 5))
	float BreathRecoveryTime{ 0.0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 5))
	float CurrentBreathRecoveryTime{ 0.0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 1))
	int CurrentReleaseBreaths{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 1))
	int CurrentRecoveryBreaths{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FALSXTTargetBreathState TargetState;
};

