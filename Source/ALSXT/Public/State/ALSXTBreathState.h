#pragma once

#include "GameplayTagContainer.h"
#include "Utility/ALSXTGameplayTags.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 100))
	float TransitionRate{ 1 };

	bool operator==(const FALSXTTargetBreathState& other) const
	{
		return (other.HoldingBreath == HoldingBreath) && (other.Rate == Rate) && (other.Alpha == Alpha) && (other.TransitionRate == TransitionRate);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTBreathState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Breath Type"))
	FGameplayTag BreathType{ ALSXTBreathTypeTags::Regular };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Holding Breath"))
	FGameplayTag HoldingBreath{ ALSXTHoldingBreathTags::False };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 30))
	float CurrentMaxHoldBreathTime{ 6.0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 30))
	float CurrentHoldBreathTime{ 6.0 };
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 3))
	float CurrentBreathRate { 1.0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 3))
	float PreviousBreathRate{ 1.0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 1))
	float CurrentBreathAlpha { 0.75 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 1))
	float PreviousBreathAlpha{ 0.75 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 30))
	float BreathRecoveryTime{ 0.0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 30))
	float CurrentBreathRecoveryTime{ 0.0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FALSXTTargetBreathState TargetState;

	bool operator==(const FALSXTBreathState& other) const
	{
		return (other.BreathType == BreathType) && (other.HoldingBreath == HoldingBreath) && (other.CurrentMaxHoldBreathTime == CurrentMaxHoldBreathTime) && (other.CurrentHoldBreathTime == CurrentHoldBreathTime) && (other.CurrentBreathRate == CurrentBreathRate) && (other.PreviousBreathRate == PreviousBreathRate) && (other.CurrentBreathAlpha == CurrentBreathAlpha) && (other.PreviousBreathAlpha == PreviousBreathAlpha) && (other.BreathRecoveryTime == BreathRecoveryTime) && (other.CurrentBreathRecoveryTime == CurrentBreathRecoveryTime) && (other.TargetState == TargetState);
	}
};

