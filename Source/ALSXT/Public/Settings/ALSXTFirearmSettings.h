#pragma once

#include "Utility/ALSXTStructs.h"
#include "Engine/DataAsset.h"
#include "Engine/EngineTypes.h"
#include "Curves/CurveVector.h"
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
class ALSXT_API UALSXTFirearmRoundAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
	FGameplayTag Caliber{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
	FGameplayTag SonicType{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
	FGameplayTag NonLethal{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
	float BaseWeight { 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
	float BaseVelocity { 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
	float BasePower { 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
	int DragCoEfficient { 700 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
	float BaseMaxEffectiveRange { 550.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
	float BaseMaxProjectileRangee { 900.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
	FGameplayTagContainer Features{FGameplayTag::EmptyTag};

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
struct ALSXT_API FALSXTFirearmRound
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	FGameplayTag Condition{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	TObjectPtr<UALSXTFirearmRoundAsset> RoundType {nullptr};
	
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTFirearmRoundState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	FGameplayTag Condition{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float Emmission{0.0f};
	
};

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UALSXTFirearmMagazineAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Format")
	FGameplayTag Name {FGameplayTag::EmptyTag};
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Format")
	FGameplayTagContainer Calibers {FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Details")
	TEnumAsByte<EPhysicalSurface> SurfaceType {SurfaceType_Default};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0), Category = "Format")
	int Capacity{ 8 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	FGameplayTag Quality{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	FGameplayTag Durability{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
	FGameplayTagContainer CheckAmmoAnimations{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
	FGameplayTagContainer RackAnimations{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
	FGameplayTagContainer ReloadAnimations{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
	FGameplayTagContainer ReloadWithRetentionAnimations{FGameplayTag::EmptyTag};

	// Conditions this weapon can be spawned in
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	FGameplayTagContainer SpawnedConditions{FGameplayTag::EmptyTag};

};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTFirearmMagazine
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Details")
	FGameplayTag Colorway{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Format")
	TObjectPtr<UALSXTFirearmMagazineAsset> MagazineType {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FGameplayTagContainer Attachments {FGameplayTag::EmptyTag};
	
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTFirearmMagazineState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	FGameplayTag Condition{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	TArray<FALSXTFirearmRound> Rounds;
	
};

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UALSXTFirearmComponentAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Format")
	FGameplayTag Name{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Format")
	FGameplayTag Type {FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
	float BaseWeight { 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Details")
	TEnumAsByte<EPhysicalSurface> SurfaceType {SurfaceType_Default};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	FGameplayTag Quality{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	FGameplayTag Durability{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Format")
	FGameplayTagContainer Features{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
	float Power { 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
	float Accuracy { 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
	float Distance { 1.0f };

};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTFirearmComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Details")
	FGameplayTag Colorway{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Format")
	TObjectPtr<UALSXTFirearmComponentAsset> Component {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FGameplayTagContainer Attachments {FGameplayTag::EmptyTag};
	
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTFirearmComponentState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	FGameplayTag Condition{FGameplayTag::EmptyTag};	
};

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UALSXTFirearmAttachmentAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Format")
	FGameplayTag Name{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Format")
	FGameplayTag Type{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
	float BaseWeight { 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Details")
	TEnumAsByte<EPhysicalSurface> SurfaceType {SurfaceType_Default};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	FGameplayTag Quality{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	FGameplayTag Durability{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Format")
	FGameplayTagContainer Features{FGameplayTag::EmptyTag};

};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTFirearmAttachment
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Details")
	FGameplayTag Colorway{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Format")
	TObjectPtr<UALSXTFirearmRailAsset> RailType {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FGameplayTagContainer Attachments {FGameplayTag::EmptyTag};
	
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTFirearmAttachmentState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	int Rail{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	FGameplayTag Condition{FGameplayTag::EmptyTag};	
};

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UALSXTFirearmRailAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Format")
	FGameplayTag Name{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Format")
	FGameplayTag Type{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
	float BaseWeight { 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Format")
	int SlotMin{4};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Format")
	int SlotMax{35};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Details")
	TEnumAsByte<EPhysicalSurface> SurfaceType {SurfaceType_Default};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	FGameplayTag Quality{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	FGameplayTag Durability{FGameplayTag::EmptyTag};

};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTFirearmRail
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Details")
	FGameplayTag Colorway{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Format")
	TObjectPtr<UALSXTFirearmRailAsset> RailType {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Format")
	int Slots{5};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	TArray<FALSXTFirearmAttachment> Attachments;
	
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTFirearmRailState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	FGameplayTag Location{FGameplayTag::EmptyTag};	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	FGameplayTag Condition{FGameplayTag::EmptyTag};	
};

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UALSXTOpticAttachmentAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Format")
	FGameplayTag Name{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Format")
	FGameplayTag Type{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
	float BaseWeight { 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Details")
	TEnumAsByte<EPhysicalSurface> SurfaceType {SurfaceType_Default};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	FGameplayTag Quality{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	FGameplayTag Durability{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Format")
	FGameplayTagContainer Features{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
	TArray<float> Magnifications;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
	FGameplayTagContainer AdjustmentAnimations{FGameplayTag::EmptyTag};

};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTOpticAttachment
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Details")
	FGameplayTag Colorway{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Format")
	TObjectPtr<UALSXTOpticAttachmentAsset> OpticType {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FGameplayTagContainer Attachments {FGameplayTag::EmptyTag};
	
};

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UALSXTGripAttachmentAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Format")
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Format")
	FGameplayTag AttachmentType{ FGameplayTag::EmptyTag };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Format")
	FGameplayTagContainer MountTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Format")
	FText FullName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Format")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
	float BaseWeight { 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	int AttachmentSlotLength{ 5 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	int MountingSlotsStart{ 3 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	int MountingSlotsEnd{ 4 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Details")
	TEnumAsByte<EPhysicalSurface> SurfaceType {SurfaceType_Default};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	FGameplayTag Quality{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	FGameplayTag Durability{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
	FALSXTWeaponActionSound AdjustmentPoses;

};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTGripAttachment
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Details")
	FGameplayTag Colorway{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Format")
	TObjectPtr<UALSXTGripAttachmentAsset> GripType {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FGameplayTagContainer Attachments {FGameplayTag::EmptyTag};
	
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTGripAttachmentState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	FGameplayTag Condition{FGameplayTag::EmptyTag};	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	int Position{0};	
};

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UALSXTFirearmMuzzleAttachmentAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Format")
	FGameplayTag Name{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Format")
	FGameplayTag Type{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
	FGameplayTag SonicType{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Details")
	TEnumAsByte<EPhysicalSurface> SurfaceType {SurfaceType_Default};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	FGameplayTag Quality{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	FGameplayTag Durability{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Format")
	FGameplayTagContainer Features{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
	float FlashSuppression { 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
	float Power { 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
	float Accuracy { 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
	float Distance { 1.0f };

};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTFirearmMuzzleAttachment
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Details")
	FGameplayTag Colorway{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Format")
	TObjectPtr<UALSXTFirearmMuzzleAttachmentAsset> MuzzleType {nullptr};
	
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTFirearmMuzzleAttachmentState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	FGameplayTag Condition{FGameplayTag::EmptyTag};	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	FGameplayTag SuppressionCondition{FGameplayTag::EmptyTag};	
};

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UALSXTFirearmPlatformAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Format")
	FGameplayTag Name{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Format")
	FGameplayTag Type{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Format")
	FGameplayTag Overlay{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Format")
	FGameplayTagContainer Caiibers{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Format")
	FGameplayTagContainer FireModes{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	FGameplayTagContainer ViewModes{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	bool UseLeftHandIK {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	FGameplayTag Quality{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	FGameplayTag Durability{FGameplayTag::EmptyTag};

	// Conditions this weapon can be spawned in
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	FGameplayTagContainer SpawnedConditions{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
	float BaseWeight { 550.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
	int BaseRoundsPerMinute { 700 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
	float BaseMaxEffectiveRange { 550.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
	float BaseMaxProjectileRangee { 900.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
	FGameplayTagContainer Components{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
	FGameplayTagContainer EquipAnimations{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
	FGameplayTagContainer UnequipAnimations{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
	FGameplayTagContainer RackAnimations{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
	FGameplayTagContainer MeleeAttackAnimations{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
	FGameplayTagContainer ClearJamAnimations{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
	FGameplayTagContainer InspectAnimations{FGameplayTag::EmptyTag};

};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTFirearm
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Details")
	FGameplayTag Colorway{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Format")
	TObjectPtr<UALSXTFirearmPlatformAsset> FirearmType {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Format")
	int Slots{5};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	TArray<FALSXTFirearmComponent> Components;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FGameplayTagContainer RailInfo {FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	TArray<FALSXTFirearmAttachment> Attachments;
	
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTFirearmParameters
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	FGameplayTag Condition{FGameplayTag::EmptyTag};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FALSXTFirearmRound Chamber;
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTGeneralFirearmSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Features", Meta = (AllowPrivateAccess))
	bool bEnable{ true };

};