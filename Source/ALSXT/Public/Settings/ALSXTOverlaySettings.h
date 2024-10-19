#pragma once

#include "GameplayTagContainer.h"
#include "Camera/CameraShakeBase.h"
#include "Animation/AnimInstance.h"
#include "ALSXTOverlaySettings.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTCameraShakeSetting
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UCameraShakeBase> CameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseMagnitude{ 1.0f };
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTOverlayCameraShakeSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseCameraShakeForOverlayMode{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FALSXTCameraShakeSetting WalkingCameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FALSXTCameraShakeSetting RunningCameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FALSXTCameraShakeSetting SprintingCameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FALSXTCameraShakeSetting CrouchWalkingCameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FALSXTCameraShakeSetting CrouchRunningCameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FALSXTCameraShakeSetting ProneWalkingCameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FALSXTCameraShakeSetting ProneRunningCameraShake;
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTOverlaySettings
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
	TMap<FGameplayTag, FALSXTOverlayCameraShakeSettings> CameraShake;
};
