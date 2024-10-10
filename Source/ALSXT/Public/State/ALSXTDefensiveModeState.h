#pragma once

#include "NativeGameplayTags.h"
#include "Utility/ALSXTGameplayTags.h"
#include "ALSXTDefensiveModeState.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTDefensiveModeState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag Mode {FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimSequenceBase> AnticipationPose{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimSequenceBase> ObstaclePose{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimSequenceBase> Montage{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer ObstacleSide{ FGameplayTag::EmptyTag };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer AnticipationSide{ FGameplayTag::EmptyTag };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag Side{ FGameplayTag::EmptyTag };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag Form{ FGameplayTag::EmptyTag };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag Velocity{ FGameplayTag::EmptyTag };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform Transform;

	bool operator==(const FALSXTDefensiveModeState& other) const
	{
		return (other.Mode == Mode) && (other.Montage == Montage) && (other.Side == Side) && (other.Form == Form) && (other.Velocity == Velocity) && (other.Transform.GetLocation() == Transform.GetLocation()) && (other.Transform.GetRotation() == Transform.GetRotation());
	}
};

