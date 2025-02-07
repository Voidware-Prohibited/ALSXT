#pragma once

#include "NativeGameplayTags.h"
#include "InputActionValue.h"
#include "InputAction.h"
#include "Animation/AnimMontage.h"
#include "ALSXTInputStructs.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (AllowPrivateAccess))
	UInputAction* InputAction {nullptr};

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (AllowPrivateAccess))
	// FInputActionValue InputActionValue;

	bool operator==(const FALSXTInputAction& other) const
	{
		return (other.InputAction == InputAction);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTInputComboEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (AllowPrivateAccess))
	TArray<FALSXTInputAction> InputActions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (Categories = "Als.Overlays", AllowPrivateAccess))
	FGameplayTagContainer Overlays;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Criteria", Meta = (AllowPrivateAccess))
	UAnimMontage* Montage {nullptr};

	bool operator==(const FALSXTInputComboEntry& other) const
	{
		return (other.InputActions == InputActions) && (other.Overlays == Overlays) && (other.Montage == Montage);
	}
};