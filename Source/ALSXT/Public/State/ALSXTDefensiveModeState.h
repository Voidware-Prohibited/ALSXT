#pragma once

#include "NativeGameplayTags.h"
#include "ALSXTDefensiveModeState.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTDefensiveModeState
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FGameplayTag Mode{ FGameplayTag::EmptyTag };

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector Location{ FVector::ZeroVector };
};

