#pragma once

#include "ALSXTEnums.generated.h"

UENUM(BlueprintType)
enum class ETargetTraceDirection : uint8
{
	Initial	UMETA(DisplayName = "Initial"),
	Left	UMETA(DisplayName = "Left"),
	Right	UMETA(DisplayName = "Right"),
	Count UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(ETargetTraceDirection, ETargetTraceDirection::Count);