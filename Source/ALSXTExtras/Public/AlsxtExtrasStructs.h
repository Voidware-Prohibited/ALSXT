#pragma once

#include "AlsxtExtrasStructs.generated.h"

USTRUCT(BlueprintType)
struct ALSXTEXTRAS_API FBlockSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float GridSize {100.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float SubGridNumber {5.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	bool ObjectAligned {true};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	bool HeightTextVisible {true};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess, EditCondition="HeightTextVisible"))
	FLinearColor HeightTextColor {1.0f, 1.0f, 1.0f, 1.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	bool DepthTextVisible {true};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess, EditCondition="DepthTextVisible"))
	FLinearColor DepthTextColor {1.0f, 1.0f, 1.0f, 1.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FLinearColor SurfaceColor {0.18f, 0.18f, 0.18f, 1.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	bool UseTopSurfaceColor {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess, EditCondition="UseTopSurfaceColor"))
	FLinearColor TopSurfaceColor {0.048f, 0.048f, 0.048f, 1.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FLinearColor GridColor {0.026042f, 0.026042f, 0.026042f, 1.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	bool UseGridEmissive {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess, EditCondition="UseGridEmissive"))
	float GridEmissiveAmount  {1.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	FLinearColor SubGridColor {0.088542f, 0.088542f, 0.088542f, 1.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	bool UseSubGridEmissive {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess, EditCondition="UseSubGridEmissive"))
	float SubGridEmissiveAmount {1.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	bool UseEdgeColor {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess, EditCondition="UseEdgeColor"))
	FLinearColor EdgeColor {1.0f, 1.0f, 1.0f, 1.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess, EditCondition="UseEdgeColor"))
	bool UseEdgeEmissive {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess, EditCondition="UseEdgeEmissive"))
	float EdgeEmissiveAmount {1.0f};

	bool operator==(const FBlockSettings& other) const
	{
		return (other.GridSize == GridSize) && (other.SubGridNumber == SubGridNumber);
	}
};