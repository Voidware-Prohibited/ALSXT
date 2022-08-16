// MIT

#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"
#include "Chaos/ChaosEngineInterface.h"
#include "Engine/DataAsset.h"
#include "Engine/EngineTypes.h"
#include "State/ALSXTFootstepState.h"
#include "ALSXTAnimNotify_SlideEffects.generated.h"

class USoundBase;
class UMaterialInterface;
class UNiagaraSystem;

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTSlideEffectSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TSoftObjectPtr<USoundBase> Sound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	EALSXTFootstepSoundSpawnType SoundSpawnType {EALSXTFootstepSoundSpawnType::SpawnAtTraceHitLocation};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle System")
	TSoftObjectPtr<UNiagaraSystem> ParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle System")
	EALSXTFootstepParticleEffectSpawnType ParticleSystemSpawnType {EALSXTFootstepParticleEffectSpawnType::SpawnAtTraceHitLocation};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle System")
	FVector ParticleSystemLocationOffset {ForceInit};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal")
	TSoftObjectPtr<UMaterialInterface> DecalMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal")
	UTexture2D* TransferTexture = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal")
	UTexture2D* TransferNormalTexture = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal")
	UTexture2D* TransferDetailTexture = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal", Meta = (ClampMin = 0))
	float TransferDetailTextureScale{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal", Meta = (ClampMin = 0))
	float SurfaceTransferAmount{ 0.5f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal", Meta = (ClampMin = 0))
	float SurfaceTransferAcceptanceAmount{ 0.5f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal", Meta = (ClampMin = -2))
	float SurfaceTransferAcceptanceNormalScale{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal", Meta = (ClampMin = 0))
	float TransferAmount{ 0.5f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal", Meta = (ClampMin = 0))
	float TransferSaturationRate{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal", Meta = (ClampMin = 0))
	float TransferDesaturationRate{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal", Meta = (ClampMin = 0.0001))
	float TransferDetailScale{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal", Meta = (ClampMin = 0))
	float TransferDetailNormalAmount{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal", Meta = (ClampMin = -2))
	float TransferNormalScale{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal", Meta = (AllowPreserveRatio))
	FLinearColor TransferPrimaryColor {0.0f, 0.0f, 0.0f, 1.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal", Meta = (AllowPreserveRatio))
	FLinearColor TransferSecondaryColor {0.0f, 0.0f, 0.0f, 1.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal", Meta = (ClampMin = 0))
	float TransferGrainSize{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal", Meta = (ClampMin = 0))
	float TransferWetness{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal", Meta = (ClampMin = 0))
	float TransferEmissive{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle System")
	FRotator ParticleSystemFootLeftRotationOffset{ForceInit};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle System")
	FRotator ParticleSystemFootRightRotationOffset{ForceInit};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal", Meta = (AllowPreserveRatio))
	FVector DecalSize {10.0f, 20.0f, 20.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal", Meta = (ClampMin = 0, ForceUnits = "s"))
	float DecalDuration{ 4.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Values", Meta = (AllowPrivateAccess))
	float DecalDurationModifierMin{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Values", Meta = (AllowPrivateAccess))
	float DecalDurationModifierMax{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal", Meta = (ClampMin = 0, ForceUnits = "s"))
	float DecalFadeOutDuration{ 2.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal")
	EALSXTFootstepDecalSpawnType DecalSpawnType {EALSXTFootstepDecalSpawnType::SpawnAttachedToTraceHitComponent};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal")
	FVector DecalLocationOffset {0.0f, -10.0f, -1.75f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal")
	FRotator DecalFootLeftRotationOffset {90.0f, -90.0f, 180.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal")
	FRotator DecalFootRightRotationOffset {-90.0f, 90.0f, 0.0f};
};

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UALSXTSlideEffectsSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TEnumAsByte<ETraceTypeQuery> SurfaceTraceChannel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (ClampMin = 0, ForceUnits = "cm"))
	float SurfaceTraceDistance{50.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName = "Foot Left Y Axis")
	FVector FootLeftYAxis {0.0f, 0.0f, 1.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName = "Foot Left Z Axis")
	FVector FootLeftZAxis {1.0f, 0.0f, 0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName = "Foot Right Y Axis")
	FVector FootRightYAxis {0.0f, 0.0f, 1.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName = "Foot Right Z Axis")
	FVector FootRightZAxis {-1.0f, 0.0f, 0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (ForceInlineRow))
	TMap<TEnumAsByte<EPhysicalSurface>, FALSXTSlideEffectSettings> Effects;
};

/**
 * 
 */
UCLASS(DisplayName = "ALSXT Slide Effects Animation Notify",
	AutoExpandCategories = ("Settings|Sound", "Settings|Decal", "Settings|Particle System"), Meta = (ShowWorldContextPin))
class ALSXT_API UALSXTAnimNotify_SlideEffects : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual FString GetNotifyName_Implementation() const override;

	virtual void Notify(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	UALSXTSlideEffectsSettings* SlideEffectsSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Sound", Meta = (AllowPrivateAccess, ClampMin = 0, ForceUnits = "x"))
	float SoundVolumeMultiplier{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Sound", Meta = (AllowPrivateAccess, ClampMin = 0, ForceUnits = "x"))
	float SoundPitchMultiplier{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	EALSXTFootBone FootBone {EALSXTFootBone::Left};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Sound", Meta = (AllowPrivateAccess))
	bool bSkipEffectsWhenInAir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Sound", Meta = (AllowPrivateAccess))
	bool bSpawnSound{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Sound", Meta = (AllowPrivateAccess))
	bool bIgnoreSlideSoundBlockCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Particle System", Meta = (AllowPrivateAccess))
	bool bSpawnParticleSystem{ true };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	FHitResult HitResult;
	
};
