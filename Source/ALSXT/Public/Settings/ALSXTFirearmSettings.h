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

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UALSXTFirearmRecoilAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil", Meta = (AllowPrivateAccess))
	FALSXTRecoilCurveSet SingleFireMeshRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	ECameraRecoilMode SingleFireCameraRecoilMode{ ECameraRecoilMode::CameraShake };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil", Meta = (AllowPrivateAccess, EditCondition = "SingleFireCameraRecoilMode == ECameraRecoilMode::Curves"))
	FALSXTRecoilCurveSet SingleFireCameraRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil", Meta = (AllowPrivateAccess, EditCondition = "SingleFireCameraRecoilMode == ECameraRecoilMode::CameraShake"))
	TObjectPtr<UCameraShakeBase> SingleCameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm", Meta = (AllowPrivateAccess, EditCondition = "SingleFireCameraRecoilMode == ECameraRecoilMode::CameraShake"))
	FVector SingleFireCameraShakeMagnitude{ 1.0f, 1.0f, 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil", Meta = (AllowPrivateAccess))
	FALSXTRecoilCurveSet AutoFireMeshRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	ECameraRecoilMode AutoFireCameraRecoilMode{ ECameraRecoilMode::CameraShake };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil", Meta = (AllowPrivateAccess, EditCondition = "AutoFireCameraRecoilMode == ECameraRecoilMode::Curves"))
	FALSXTRecoilCurveSet AutoFireCameraRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil", Meta = (AllowPrivateAccess, EditCondition = "AutoFireCameraRecoilMode == ECameraRecoilMode::CameraShake"))
	TObjectPtr<UCameraShakeBase> AutoFireCameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm", Meta = (AllowPrivateAccess, EditCondition = "AutoFireCameraRecoilMode == ECameraRecoilMode::CameraShake"))
	FVector AutoFireCameraShakeMagnitude{ 1.0f, 1.0f, 1.0f };
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
struct ALSXT_API FALSXTFirearmDischargeEffectsState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	FGameplayTag State {FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	TObjectPtr<UALSXTFirearmRecoilAsset> RecoilAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
	TObjectPtr<UNiagaraSystem> Particle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", Meta = (AllowPrivateAccess))
	TObjectPtr<USoundBase> DischargeSound;

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
	USceneComponent* Component;

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