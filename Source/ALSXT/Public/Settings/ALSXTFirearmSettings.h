#pragma once

#include "Utility/ALSXTStructs.h"
#include "Engine/DataAsset.h"
#include "Engine/EngineTypes.h"
#include "Curves/CurveVector.h"
#include "NiagaraSystem.h"
#include "Sound/SoundBase.h"
#include "ALSXTFirearmSettings.generated.h"

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTRecoilCurveSet
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm", Meta = (AllowPrivateAccess))
	TObjectPtr<UCurveVector> MeshLocation{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm", Meta = (AllowPrivateAccess))
	TObjectPtr<UCurveVector> MeshRotation{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm", Meta = (AllowPrivateAccess))
	TObjectPtr<UCurveVector> CameraLocation{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm", Meta = (AllowPrivateAccess))
	TObjectPtr<UCurveVector> CameraRotation{ nullptr };

};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTRecoilCurveMagnitudeSet
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm", Meta = (AllowPrivateAccess))
	FVector MeshLocationMagnitude{ 1.0f, 1.0f, 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm", Meta = (AllowPrivateAccess))
	FVector MeshRotationMagnitude{ 1.0f, 1.0f, 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm", Meta = (AllowPrivateAccess))
	FVector CameraLocationMagnitude{ 1.0f, 1.0f, 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm", Meta = (AllowPrivateAccess))
	FVector CameraRotationMagnitude{ 1.0f, 1.0f, 1.0f };

};

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UALSXTFirearmRecoilAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil", Meta = (AllowPrivateAccess))
	FALSXTRecoilCurveSet Single;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil", Meta = (AllowPrivateAccess))
	FALSXTRecoilCurveMagnitudeSet SingleMeshMagnitude;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil", Meta = (AllowPrivateAccess))
	FALSXTRecoilCurveMagnitudeSet SingleCameraMagnitude;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil", Meta = (AllowPrivateAccess))
	FALSXTRecoilCurveSet Burst;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil", Meta = (AllowPrivateAccess))
	FALSXTRecoilCurveMagnitudeSet BurstMeshMagnitude;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil", Meta = (AllowPrivateAccess))
	FALSXTRecoilCurveMagnitudeSet BurstCameraMagnitude;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil", Meta = (AllowPrivateAccess))
	FALSXTRecoilCurveSet Auto;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil", Meta = (AllowPrivateAccess))
	FALSXTRecoilCurveMagnitudeSet AutoMeshMagnitude;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil", Meta = (AllowPrivateAccess))
	FALSXTRecoilCurveMagnitudeSet AutoCameraMagnitude;

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
	TObjectPtr<USoundBase> Sounds;

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
struct ALSXT_API FALSXTFirearmAimState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	FGameplayTag SightLocation{ FGameplayTag::EmptyTag };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Origin")
	FTransform CurrentTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float CurrentFOV;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float CurrentMagnification;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Origin")
	FTransform OriginTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Origin")
	FTransform OriginOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sight|Rear")
	FTransform RearSightTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sight|Rear")
	FName RearSightSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sight|Front")
	FTransform FrontSightTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sight|Front")
	FName FrontSightSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sight|Barrel")
	FTransform BarrelTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sight|Barrel")
	FName BarrelSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sight|Barrel")
	FVector TargetPoint{ EForceInit::ForceInit };
};

