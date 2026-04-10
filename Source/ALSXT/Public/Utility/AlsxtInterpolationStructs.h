#pragma once

#include "Kismet/KismetMathLibrary.h"
#include "AlsxtInterpolationStructs.generated.h"

class UCurveFloat;
class UCurveVector;
class UTimelineComponent;

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtEaseLerpRule
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Lerp|Ease", Meta = (AllowPrivateAccess))
	TEnumAsByte<EEasingFunc::Type> EaseFunction {EEasingFunc::Linear};

	UPROPERTY(EditAnywhere, Category = "Lerp|Ease", Meta = (AllowPrivateAccess))
	float BlendExp = 2.0f;

	bool operator==(const FAlsxtEaseLerpRule& other) const
	{
		return (other.EaseFunction == EaseFunction) && (other.BlendExp == BlendExp);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtFloatInterpolationRule
{
	GENERATED_BODY()

	// Advanced Curve Asset Interpolation.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TSoftObjectPtr<UCurveFloat> CurveFloat {nullptr};

	// Basic Ease Function Interpolation. Only Utilized when CurveFloat is Empty
	UPROPERTY(EditAnywhere, Category = "Lerp|Ease", Meta = (AllowPrivateAccess, ForceInlineRow, EditCondition="CurveFloat.IsNull()"))
	FAlsxtEaseLerpRule Ease {FAlsxtEaseLerpRule()};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float Duration {1.0f};

	bool operator==(const FAlsxtFloatInterpolationRule& other) const
	{
		return (other.CurveFloat == CurveFloat) && (other.Ease == Ease) && (other.Duration == Duration);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtVectorInterpolationRule
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float Duration {1.0f};

	// Advanced Curve Asset Interpolation.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TSoftObjectPtr<UCurveVector> CurveVector {nullptr};

	// Basic Ease Function Interpolation. Only Utilized when CurveVector is Empty
	UPROPERTY(EditAnywhere, Category = "Lerp|Ease", Meta = (AllowPrivateAccess, ForceInlineRow, EditCondition="CurveVector.IsNull()"))
	FAlsxtEaseLerpRule Ease {FAlsxtEaseLerpRule()};

	bool operator==(const FAlsxtVectorInterpolationRule& other) const
	{
		return (other.Duration == Duration) && (other.CurveVector == CurveVector) && (other.Ease == Ease);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtFloatInterpolationParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float Value {0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FName ParameterName {FName()};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess, ForceInlineRow))
	FAlsxtFloatInterpolationRule InterpolationRule {FAlsxtFloatInterpolationRule()};

	bool operator==(const FAlsxtFloatInterpolationParams& other) const
	{
		return (other.Value == Value) && (other.ParameterName == ParameterName) && (other.InterpolationRule == InterpolationRule);
	}
};

//
USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtVectorInterpolationParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FVector Value {FVector::ZeroVector};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess, ForceInlineRow))
	FAlsxtVectorInterpolationRule InterpolationRule {FAlsxtVectorInterpolationRule()};
	
	bool operator==(const FAlsxtVectorInterpolationParams& other) const
	{
		return (other.Value == Value) && (other.InterpolationRule == InterpolationRule);
	}
};

USTRUCT(BlueprintType)
struct FAlsxtFloatInterpolationState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FAlsxtFloatInterpolationParams FloatInterpolationParams {FAlsxtFloatInterpolationParams()};

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	UTimelineComponent* Timeline {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float CurrentValue {0.0f};

	// Start value is tracked to set up Lerp correctly
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float StartValue {0.0f};

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FTimerHandle TimelineTimerHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	bool bIsActive {false};

	bool operator==(const FAlsxtFloatInterpolationState& other) const
	{
		return (other.FloatInterpolationParams == FloatInterpolationParams) && (other.Timeline == Timeline);
	}
};

USTRUCT(BlueprintType)
struct FAlsxtVectorInterpolationState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FAlsxtVectorInterpolationParams VectorInterpolationParams {FAlsxtVectorInterpolationParams()};

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	UTimelineComponent* Timeline  {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FVector CurrentValue {FVector::ZeroVector};

	// Start value is tracked to set up Lerp correctly
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FVector StartValue {FVector::ZeroVector};

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FTimerHandle TimelineTimerHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	bool bIsActive {false};

	bool operator==(const FAlsxtVectorInterpolationState& other) const
	{
		return (other.VectorInterpolationParams == VectorInterpolationParams) && (other.Timeline == Timeline);
	}
};

USTRUCT(BlueprintType)
struct FAlsxtLerpParamQueue_ScalarVector
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TArray<FAlsxtFloatInterpolationParams> Scalar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TArray<FAlsxtVectorInterpolationParams> Vector;

	bool operator==(const FAlsxtLerpParamQueue_ScalarVector& other) const
	{
		return (other.Scalar == Scalar) && (other.Vector == Vector);
	}
};

USTRUCT(BlueprintType)
struct FAlsxtLerpState_ScalarVector
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TArray<FAlsxtFloatInterpolationState> Scalar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TArray<FAlsxtVectorInterpolationState> Vector;

	bool operator==(const FAlsxtLerpState_ScalarVector& other) const
	{
		return (other.Scalar == Scalar) && (other.Vector == Vector);
	}
};
