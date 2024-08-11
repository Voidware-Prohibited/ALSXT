#pragma once

#include "NativeGameplayTags.h"
#include "Utility/ALSXTGameplayTags.h"
#include "ALSXTDefensiveModeState.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTDefensiveModeState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag Mode{ ALSXTDefensiveModeTags::None };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimSequenceBase> Montage{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag Side{ FGameplayTag::EmptyTag };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag Form{ FGameplayTag::EmptyTag };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag Velocity{ FGameplayTag::EmptyTag };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform Transform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location{ FVector::ZeroVector };
};

