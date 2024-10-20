#pragma once

#include "ALSXTCollisionStructs.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTPlaceholderCollisionStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria")
	bool NewProperty;

	bool operator==(const FALSXTPlaceholderCollisionStruct& other) const
	{
		return (other.NewProperty == NewProperty);
	}
};