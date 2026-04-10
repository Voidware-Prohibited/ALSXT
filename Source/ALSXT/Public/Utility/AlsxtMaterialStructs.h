#pragma once

#include "AlsxtInterpolationStructs.h"
#include "Engine/EngineTypes.h"
#include "Kismet/KismetMathLibrary.h"
#include "AlsxtMaterialStructs.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtMaterialParameterScalar
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FName ParameterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float Value;

	bool operator==(const FAlsxtMaterialParameterScalar& other) const
	{
		return (other.ParameterName == ParameterName) && (other.Value == Value);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtMaterialParameterVector
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FName ParameterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FVector Value;

	bool operator==(const FAlsxtMaterialParameterVector& other) const
	{
		return (other.ParameterName == ParameterName) && (other.Value == Value);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtParameterLerpScalar
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess, ForceInlineRow))
	FAlsxtMaterialParameterScalar Parameter;

	UPROPERTY(EditAnywhere, Category = "Interpolation")
	FAlsxtFloatInterpolationParams Interpolation;

	bool operator==(const FAlsxtParameterLerpScalar& other) const
	{
		return (other.Parameter == Parameter) && (other.Interpolation == Interpolation);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtParameterLerpVector
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess, ForceInlineRow))
	FAlsxtMaterialParameterVector Parameter;

	UPROPERTY(EditAnywhere, Category = "Interpolation")
	FAlsxtVectorInterpolationParams Interpolation;

	bool operator==(const FAlsxtParameterLerpVector& other) const
	{
		return (other.Parameter == Parameter) && (other.Interpolation == Interpolation);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtMaterialParametersSet
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{ParameterName}: {Value}"))
	TArray<FAlsxtParameterLerpScalar> ScalarParameters;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (TitleProperty = "{ParameterName}: {Value}"))
	TArray<FAlsxtParameterLerpVector> VectorParameters;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Reset", Meta = (AllowPrivateAccess))
	bool bResetAllParameters {false};

	// Length of time for Reset to occur
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Reset", Meta = (AllowPrivateAccess, EditCondition="bResetAllParameters"))
	float ResetDuration {0.0f};

	bool operator==(const FAlsxtMaterialParametersSet& other) const
	{
		return (other.ScalarParameters == ScalarParameters) && (other.VectorParameters == VectorParameters);
	}
};