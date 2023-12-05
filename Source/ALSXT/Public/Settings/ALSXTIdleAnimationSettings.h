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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations", Meta = (TitleProperty = "{Sex}  {Overlay} {Stance} {Injury} {Variant} {Montage}", AllowPrivateAccess))
	TArray<FIdleAnimation> IdleAnimations;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool EnableIdleCameraRotation{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float IdleCameraRotationSpeed{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool EnableIdleGazing{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinimumSpectacleScoreForGazing{ 1.0f };
};