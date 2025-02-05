#pragma once

#include "Utility/ALSXTStructs.h"
#include "Engine/DataAsset.h"
#include "Engine/EngineTypes.h"
#include "Curves/CurveVector.h"
#include "NiagaraSystem.h"
#include "Sound/SoundBase.h"
#include "ALSXTFirearmSettings.generated.h"

UENUM(BlueprintType)
enum class ECameraRecoilMode : uint8
{ 
	Curves, 
	CameraShake
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTFirearmDischargeEffectsSettings
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
struct ALSXT_API FALSXTFirearmRecoilSettings
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
struct ALSXT_API FALSXTGeneralSightsSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm", Meta = (AllowPrivateAccess))
	FName HeadSocketName{ "Head" };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm", Meta = (AllowPrivateAccess))
	FName CameraSocketName{ "Camera" };

};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTRecoilState
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
struct ALSXT_API FALSXTRecoilCurveSet
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
struct ALSXT_API FALSXTRecoilCameraShakeViewModeSet
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
struct ALSXT_API FALSXTRecoilCameraShakeSet
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil", Meta = (AllowPrivateAccess))
	FALSXTRecoilCameraShakeViewModeSet ThirdPerson;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil", Meta = (AllowPrivateAccess))
	FALSXTRecoilCameraShakeViewModeSet FirstPerson;
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTRecoilControlRotationCurveSet
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
	FALSXTRecoilControlRotationCurveSet SingleFireRecoilControlRotation{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control Rotation", Meta = (AllowPrivateAccess))
	FALSXTRecoilControlRotationCurveSet AutoFireRecoilControlRotation{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", Meta = (AllowPrivateAccess))
	FALSXTRecoilCurveSet SingleFireMeshRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", Meta = (AllowPrivateAccess))
	FALSXTRecoilCurveSet AutoFireMeshRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Settings")
	ECameraRecoilMode SingleFireCameraRecoilMode{ ECameraRecoilMode::CameraShake };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Settings")
	ECameraRecoilMode AutoFireCameraRecoilMode{ ECameraRecoilMode::CameraShake };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Camera Shake", Meta = (AllowPrivateAccess, EditCondition = "SingleFireCameraRecoilMode == ECameraRecoilMode::CameraShake"))
	FALSXTRecoilCameraShakeSet SingleCameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Camera Shake", Meta = (AllowPrivateAccess, EditCondition = "AutoFireCameraRecoilMode == ECameraRecoilMode::CameraShake"))
	FALSXTRecoilCameraShakeSet AutoFireCameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Camera Curves", Meta = (AllowPrivateAccess, EditCondition = "SingleFireCameraRecoilMode == ECameraRecoilMode::Curves"))
	FALSXTRecoilCurveSet SingleFireCameraRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Camera Curves", Meta = (AllowPrivateAccess, EditCondition = "AutoFireCameraRecoilMode == ECameraRecoilMode::Curves"))
	FALSXTRecoilCurveSet AutoFireCameraRecoil;
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTMuzzleFlash
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
struct ALSXT_API FALSXTFirearmDischargeSound
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
struct ALSXT_API FALSXTFirearmDischargeEffectsModifiersState
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
class ALSXT_API UALSXTFirearmRoundEffectsAsset : public UDataAsset
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
struct ALSXT_API FALSXTFirearmDischargeEffectsState
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
struct ALSXT_API FALSXTFirearmActionSound
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
struct ALSXT_API FALSXTFirearmSightsInfo
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
struct ALSXT_API FALSXTFirearmSightsState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	FGameplayTag DefaultSightLocation{ FGameplayTag::EmptyTag };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	FGameplayTagContainer AvailableSightLocations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sight")
	TArray<FALSXTFirearmSightsInfo> Sights;
};


USTRUCT(BlueprintType)
struct ALSXT_API FALSXTFirearmAimState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	FALSXTFirearmSightsInfo CurrentSight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = -180, ClampMax = 180, ForceUnits = "deg"))
	FRotator ControlRotation{ EForceInit::ForceInit };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	FTransform TargetTransform;
};