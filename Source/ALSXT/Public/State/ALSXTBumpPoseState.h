#pragma once

#include "NativeGameplayTags.h"
#include "Utility/ALSXTGameplayTags.h"
#include "ALSXTBumpPoseState.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTBumpPoseState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* Actor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag Mode{ FGameplayTag::EmptyTag };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimSequenceBase> Pose{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Alpha{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag Velocity{ FGameplayTag::EmptyTag };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag Side{ FGameplayTag::EmptyTag };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag Form{ FGameplayTag::EmptyTag };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location{ FVector::ZeroVector };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform Transform;
};

