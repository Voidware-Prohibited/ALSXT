#pragma once

#include "Utility/ALSXTStructs.h"
#include "Utility/ALSXTGameplayTags.h"
#include "NativeGameplayTags.h"
#include "ALSXTIdleAnimationSettings.generated.h"

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UALSXTIdleAnimationSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations", Meta = (TitleProperty = "{Sex} {Overlay} {Stance} {Injury} {Variant} {Montage}", AllowPrivateAccess))
	TArray<FIdleAnimation> IdleAnimations;

};


USTRUCT(BlueprintType)
struct ALSXT_API FALSXTIdleState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIdle {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Alss.Idle Mode"))
	FGameplayTag Mode{ FGameplayTag::EmptyTag };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* CurrentIdleMontage {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0))
	float TargetTime{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0))
	float CurrentTime{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0))
	float TargetTimeBeforeNext{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0))
	float CurrentTimeBeforeNext{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0))
	float GazingTargetTime{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0))
	float GazingCurrentTime{ 0.0f };

	bool operator==(const FALSXTIdleState& other) const
	{
		return (other.bIdle == bIdle) && (other.Mode == Mode) && (other.CurrentIdleMontage == CurrentIdleMontage) && (other.TargetTime == TargetTime) && (other.CurrentTime == CurrentTime) && (other.TargetTimeBeforeNext == TargetTimeBeforeNext) && (other.CurrentTimeBeforeNext == CurrentTimeBeforeNext)&& (other.GazingTargetTime == GazingTargetTime) && (other.GazingCurrentTime == GazingCurrentTime);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTALSXTGeneralIdleAnimationSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool EnableIdleAnimations{ true };

	/* Idle Animations will only play in these Stamina levels */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.Stamina", TitleProperty = "{Stamina}"))
	FGameplayTagContainer EligibleStaminaLevels{ ALSXTStaminaTags::Full };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0))
	FVector2D TimeDelayBeforeIdle{ 20.0f, 30.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0))
	FVector2D TimeDelayBetweenAnimations{ 10.0f, 20.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0))
	FVector2D TimeDelayBeforeGazing{ 60.0f, 120.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool EnableIdleCameraRotation{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float IdleCameraRotationSpeed{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool EnableIdleGazing{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinimumSpectacleScoreForGazing{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDebugMode{ false };

	bool operator==(const FALSXTALSXTGeneralIdleAnimationSettings& other) const
	{
		return (other.EnableIdleAnimations == EnableIdleAnimations) && (other.EligibleStaminaLevels == EligibleStaminaLevels) && (other.TimeDelayBeforeIdle == TimeDelayBeforeIdle) && (other.TimeDelayBetweenAnimations == TimeDelayBetweenAnimations) && (other.EnableIdleCameraRotation == EnableIdleCameraRotation) && (other.IdleCameraRotationSpeed == IdleCameraRotationSpeed) && (other.EnableIdleGazing == EnableIdleGazing) && (other.MinimumSpectacleScoreForGazing == MinimumSpectacleScoreForGazing) && (other.bDebugMode == bDebugMode);
	}
};