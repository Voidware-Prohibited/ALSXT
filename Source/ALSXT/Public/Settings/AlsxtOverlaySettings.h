#pragma once

#include "GameplayTagContainer.h"
#include "Camera/CameraShakeBase.h"
#include "Animation/AnimInstance.h"
#include "AlsxtOverlaySettings.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtCameraShakeSetting
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UCameraShakeBase> CameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseMagnitude{ 1.0f };

	bool operator==(const FAlsxtCameraShakeSetting& other) const
	{
		return (other.CameraShake == CameraShake) && (other.BaseMagnitude == BaseMagnitude);
	}
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtOverlayCameraShakeSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseCameraShakeForOverlayMode{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAlsxtCameraShakeSetting WalkingCameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAlsxtCameraShakeSetting ADSCameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAlsxtCameraShakeSetting CombatCameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAlsxtCameraShakeSetting RunningCameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAlsxtCameraShakeSetting SprintingCameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAlsxtCameraShakeSetting CrouchWalkingCameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAlsxtCameraShakeSetting CrouchADSCameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAlsxtCameraShakeSetting CrouchCombatCameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAlsxtCameraShakeSetting CrouchRunningCameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAlsxtCameraShakeSetting ProneWalkingCameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAlsxtCameraShakeSetting ProneCombatCameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAlsxtCameraShakeSetting ProneRunningCameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAlsxtCameraShakeSetting ProneSprintingCameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAlsxtCameraShakeSetting LadderWalkingCameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAlsxtCameraShakeSetting LadderRunningCameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAlsxtCameraShakeSetting LadderSprintingCameraShake;

};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtOverlaySettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Als.OverlayMode"))
	TMap<FGameplayTag, TSubclassOf<UAnimInstance>> OverlayAnimationInstanceClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer AimableOverlayModes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseGunBoneForOverlayObjects {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer LeftIKOverlayModes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGameplayTag, FAlsxtOverlayCameraShakeSettings> CameraShake;
};
