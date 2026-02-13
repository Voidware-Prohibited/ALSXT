#pragma once

#include "Utility/AlsxtStructs.h"
#include "Engine/DataAsset.h"
#include "Engine/EngineTypes.h"
#include "Curves/CurveVector.h"
#include "NiagaraSystem.h"
#include "Sound/SoundBase.h"
#include "AlsxtFirearmSettings.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtFirearmDischargeSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm", Meta = (AllowPrivateAccess, Categories = "Alsxt.Firearm Fire Mode"))
	FGameplayTagContainer FireModes;

	/** Fire Rate in shots per minute */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm", Meta = (AllowPrivateAccess))
	float FireRate {0.0f};

	/** Rounds fired in Burst Fire mode */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm", Meta = (AllowPrivateAccess))
	int BurstFireAmount {0};
};

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UAlsxtFirearmDischargeSettingsDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm", Meta = (AllowPrivateAccess, Categories = "Alsxt.Firearm Fire Mode"))
	FGameplayTagContainer FireModes;

	/** Fire Rate in Rounds per Minute (RPM) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm", Meta = (AllowPrivateAccess))
	float FireRate {600.0f};

	/** Rounds fired in Burst Fire mode */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm", Meta = (AllowPrivateAccess))
	int BurstFireAmount {3};
};

UENUM(BlueprintType)
enum class EAlsxtCameraRecoilMode : uint8
{ 
	Curves, 
	CameraShake
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtFirearmDischargeEffectsSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm", Meta = (AllowPrivateAccess))
	bool bMuzzleEffects{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm", Meta = (AllowPrivateAccess))
	bool bEnableSoundEffects{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm", Meta = (AllowPrivateAccess))
	bool bEnableEjectionPortEffects{ true };
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtFirearmRecoilSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm", Meta = (AllowPrivateAccess))
	bool bEnableMeshAnimationRecoil{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm", Meta = (AllowPrivateAccess))
	bool bEnableControlRotationRecoil{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm", Meta = (AllowPrivateAccess))
	bool bEnableCameraShakeRecoil{ true };
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtGeneralSightsSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm", Meta = (AllowPrivateAccess))
	FName HeadSocketName{ "Head" };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm", Meta = (AllowPrivateAccess))
	FName CameraSocketName{ "Camera" };

};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtRecoilState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm", Meta = (AllowPrivateAccess))
	bool RecoilPlaying{ false };
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm", Meta = (AllowPrivateAccess))
	FTransform PivotPointTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm", Meta = (AllowPrivateAccess))
	FTransform RecoilTransform;

};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtRecoilCurveSet
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm", Meta = (AllowPrivateAccess))
	TObjectPtr<UCurveVector> Location{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm", Meta = (AllowPrivateAccess))
	FVector LocationMagnitude{ 1.0f, 1.0f, 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm", Meta = (AllowPrivateAccess))
	TObjectPtr<UCurveVector> Rotation{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm", Meta = (AllowPrivateAccess))
	FVector RotationMagnitude{ 1.0f, 1.0f, 1.0f };

};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtRecoilCameraShakeViewModeSet
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil", Meta = (AllowPrivateAccess))
	TSubclassOf<UCameraShakeBase> CameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil", Meta = (AllowPrivateAccess))
	float CameraShakeMagnitude{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil", Meta = (AllowPrivateAccess))
	TSubclassOf<UCameraShakeBase> AimedCameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil", Meta = (AllowPrivateAccess))
	float AimedCameraShakeMagnitude{ 1.0f };
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtRecoilCameraShakeSet
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil", Meta = (AllowPrivateAccess))
	FAlsxtRecoilCameraShakeViewModeSet ThirdPerson;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil", Meta = (AllowPrivateAccess))
	FAlsxtRecoilCameraShakeViewModeSet FirstPerson;
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtRecoilControlRotationCurveSet
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Uaimed", Meta = (AllowPrivateAccess))
	TObjectPtr<UCurveVector> Unaimed{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Uaimed", Meta = (AllowPrivateAccess))
	float UnaimedMagnitude{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aimed", Meta = (AllowPrivateAccess))
	TObjectPtr<UCurveVector> Aimed{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aimed", Meta = (AllowPrivateAccess))
	float AimedMagnitude{ 1.0f };

};

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UALSXTFirearmRecoilAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control Rotation", Meta = (AllowPrivateAccess))
	FAlsxtRecoilControlRotationCurveSet SingleFireRecoilControlRotation{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control Rotation", Meta = (AllowPrivateAccess))
	FAlsxtRecoilControlRotationCurveSet AutoFireRecoilControlRotation{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", Meta = (AllowPrivateAccess))
	FAlsxtRecoilCurveSet SingleFireMeshRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", Meta = (AllowPrivateAccess))
	FAlsxtRecoilCurveSet AutoFireMeshRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Settings")
	EAlsxtCameraRecoilMode SingleFireCameraRecoilMode{ EAlsxtCameraRecoilMode::CameraShake };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Settings")
	EAlsxtCameraRecoilMode AutoFireCameraRecoilMode{ EAlsxtCameraRecoilMode::CameraShake };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Camera Shake", Meta = (AllowPrivateAccess, EditCondition = "SingleFireCameraRecoilMode == EAlsxtCameraRecoilMode::CameraShake"))
	FAlsxtRecoilCameraShakeSet SingleCameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Camera Shake", Meta = (AllowPrivateAccess, EditCondition = "AutoFireCameraRecoilMode == EAlsxtCameraRecoilMode::CameraShake"))
	FAlsxtRecoilCameraShakeSet AutoFireCameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Camera Curves", Meta = (AllowPrivateAccess, EditCondition = "SingleFireCameraRecoilMode == EAlsxtCameraRecoilMode::Curves"))
	FAlsxtRecoilCurveSet SingleFireCameraRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Camera Curves", Meta = (AllowPrivateAccess, EditCondition = "AutoFireCameraRecoilMode == EAlsxtCameraRecoilMode::Curves"))
	FAlsxtRecoilCurveSet AutoFireCameraRecoil;
};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtMuzzleFlash
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Format")
	FGameplayTagContainer Calibers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Format")
	FGameplayTagContainer Modifiers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm", Meta = (AllowPrivateAccess))
	TArray<UNiagaraSystem*> Particles;

};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtFirearmDischargeSound
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Format")
	FGameplayTagContainer Calibers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Format")
	FGameplayTagContainer Modifiers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm", Meta = (AllowPrivateAccess))
	TObjectPtr<USoundBase> Sound;

};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtFirearmDischargeEffectsModifiersState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	float CalculatedRecoilAnimationModifier{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	float CalculatedRecoilControlRotationModifier{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	float CalculatedRecoilCameraShakeModifier{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
	float CalculatedMuzzleFlashParticleModifier{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	float CalculatedDischargeSoundLoudnessModifier{ 1.0f };

};

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UAlsxtFirearmRoundEffectsAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	TSoftObjectPtr<UALSXTFirearmRecoilAsset> RecoilAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	float RecoilAnimationMagnitude{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	float RecoilControlRotationMagnitude{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	float RecoilCameraShakeMagnitude{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
	TSoftObjectPtr<UNiagaraSystem> MuzzleFlashParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
	float MuzzleFlashParticleMagnitude{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
	TSoftObjectPtr<UNiagaraSystem> MuzzleExhaustParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
	TSoftObjectPtr<UNiagaraSystem> EjectionPortParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", Meta = (AllowPrivateAccess))
	TSoftObjectPtr<USoundBase> DischargeSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	float DischargeSoundLoudnessMagnitude{ 1.0f };

};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtFirearmDischargeEffectsState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	FGameplayTag State {FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	TObjectPtr<UALSXTFirearmRecoilAsset> RecoilAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	float CalculatedRecoilAnimationMagnitude{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	float CalculatedRecoilControlRotationMagnitude{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	float CalculatedRecoilCameraShakeMagnitude{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
	TObjectPtr<UNiagaraSystem> MuzzleFlashParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
	float CalculatedMuzzleFlashParticleMagnitude{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
	TObjectPtr<UNiagaraSystem> MuzzleExhaustParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
	TObjectPtr<UNiagaraSystem> EjectionPortParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", Meta = (AllowPrivateAccess))
	TObjectPtr<USoundBase> DischargeSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	float CalculatedDischargeSoundLoudnessMagnitude{ 1.0f };

};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtFirearmActionSound
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Format")
	FGameplayTagContainer Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Format")
	FGameplayTagContainer Modifiers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm", Meta = (AllowPrivateAccess))
	TObjectPtr<USoundBase> Sounds;

};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtFirearmSightsInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Format")
	FGameplayTagContainer SightLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Format")
	USceneComponent* Component  {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float BaseADSSpeed{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float CurrentADSSpeed{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float CurrentMagnification{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float AimedVignetteAmount{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float AimedExteriorBlurAmount{ 1.0f };

};

USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtFirearmSightsState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	FGameplayTag DefaultSightLocation{ FGameplayTag::EmptyTag };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	FGameplayTagContainer AvailableSightLocations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sight")
	TArray<FAlsxtFirearmSightsInfo> Sights;
};


USTRUCT(BlueprintType)
struct ALSXT_API FAlsxtFirearmAimState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	FAlsxtFirearmSightsInfo CurrentSight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = -180, ClampMax = 180, ForceUnits = "deg"))
	FRotator ControlRotation{ EForceInit::ForceInit };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	FTransform TargetTransform;
};