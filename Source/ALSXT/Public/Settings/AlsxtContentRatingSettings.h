#pragma once

#include "AlsxtContentRatingSettings.generated.h"

UENUM(BlueprintType)
enum class EIarcRating : uint8
{
	IarcRating_3 UMETA(DisplayName = "3+"),
	IarcRating_7 UMETA(DisplayName = "7+"),
	IarcRating_12 UMETA(DisplayName = "12+"),
	IarcRating_16 UMETA(DisplayName = "16+"),
	IarcRating_18 UMETA(DisplayName = "18+")
};
