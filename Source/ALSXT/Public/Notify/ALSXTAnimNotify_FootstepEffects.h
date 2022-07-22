#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"
#include "Chaos/ChaosEngineInterface.h"
#include "Engine/DataAsset.h"
#include "Engine/EngineTypes.h"
#include "ALSXTAnimNotify_FootstepEffects.generated.h"

class USoundBase;
class UMaterialInterface;
class UNiagaraSystem;

UENUM(BlueprintType)
enum class EALSXTFootBone : uint8
{
	Left,
	Right,
};

UENUM(BlueprintType)
enum class EALSXTFootstepSoundType : uint8
{
	Step,
	WalkRun,
	Land
};

UENUM(BlueprintType)
enum class EALSXTFootstepSoundSpawnType : uint8
{
	SpawnAtTraceHitLocation,
	SpawnAttachedToFootBone
};

UENUM(BlueprintType)
enum class EALSXTFootstepDecalSpawnType : uint8
{
	SpawnAtTraceHitLocation,
	SpawnAttachedToTraceHitComponent
};

UENUM(BlueprintType)
enum class EALSXTFootstepParticleEffectSpawnType : uint8
{
	SpawnAtTraceHitLocation,
	SpawnAttachedToFootBone
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTFootstepEffectSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TSoftObjectPtr<USoundBase> Sound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	EALSXTFootstepSoundSpawnType SoundSpawnType{EALSXTFootstepSoundSpawnType::SpawnAtTraceHitLocation};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal")
	TSoftObjectPtr<UMaterialInterface> DecalMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal")
	TSoftObjectPtr<UTexture2D> FootstepSoleTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal")
	TSoftObjectPtr<UTexture2D> FootstepSoleNormal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal")
	TSoftObjectPtr<UTexture2D> FootstepSoleDetail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal", Meta = (ClampMin = 0))
	float FootstepSoleDetailScale{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal", Meta = (AllowPreserveRatio))
	FLinearColor MaterialPrimaryColor{0.0f, 0.0f, 0.0f, 1.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal", Meta = (AllowPreserveRatio))
	FLinearColor MaterialSecondaryColor{0.0f, 0.0f, 0.0f, 1.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal", Meta = (ClampMin = 0))
	float MaterialGrainSize{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal", Meta = (ClampMin = 0))
	float MaterialWetness{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal", Meta = (ClampMin = 0))
	float MaterialTransferAmount{ 0.5f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal", Meta = (AllowPreserveRatio))
	FVector DecalSize{10.0f, 20.0f, 20.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal", Meta = (ClampMin = -1))
	float DecalNormalAmount{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal", Meta = (ClampMin = 0))
	float MaterialEmissive{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal", Meta = (ClampMin = 0, ForceUnits = "s"))
	float DecalDuration{4.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal", Meta = (ClampMin = 0, ForceUnits = "s"))
	float DecalFadeOutDuration{2.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal")
	EALSXTFootstepDecalSpawnType DecalSpawnType{EALSXTFootstepDecalSpawnType::SpawnAttachedToTraceHitComponent};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal")
	FVector DecalLocationOffset{0.0f, -10.0f, -1.75f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal")
	FRotator DecalFootLeftRotationOffset{90.0f, -90.0f, 180.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal")
	FRotator DecalFootRightRotationOffset{-90.0f, 90.0f, 0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle System")
	TSoftObjectPtr<UNiagaraSystem> ParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle System")
	EALSXTFootstepParticleEffectSpawnType ParticleSystemSpawnType{EALSXTFootstepParticleEffectSpawnType::SpawnAtTraceHitLocation};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle System")
	FVector ParticleSystemLocationOffset{ForceInit};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle System")
	FRotator ParticleSystemFootLeftRotationOffset{ForceInit};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle System")
	FRotator ParticleSystemFootRightRotationOffset{ForceInit};
};

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UALSXTFootstepEffectsSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TEnumAsByte<ETraceTypeQuery> SurfaceTraceChannel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (ClampMin = 0, ForceUnits = "cm"))
	float SurfaceTraceDistance{50.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName = "Foot Left Y Axis")
	FVector FootLeftYAxis{0.0f, 0.0f, 1.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName = "Foot Left Z Axis")
	FVector FootLeftZAxis{1.0f, 0.0f, 0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName = "Foot Right Y Axis")
	FVector FootRightYAxis{0.0f, 0.0f, 1.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName = "Foot Right Z Axis")
	FVector FootRightZAxis{-1.0f, 0.0f, 0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (ForceInlineRow))
	TMap<TEnumAsByte<EPhysicalSurface>, FALSXTFootstepEffectSettings> Effects;
};

UCLASS(DisplayName = "ALSXT Footstep Effects Animation Notify",
	AutoExpandCategories = ("Settings|Sound", "Settings|Decal", "Settings|Particle System"), Meta = (ShowWorldContextPin))
class ALSXT_API UALSXTAnimNotify_FootstepEffects : public UAnimNotify
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	TObjectPtr<UALSXTFootstepEffectsSettings> FootstepEffectsSettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	EALSXTFootBone FootBone {EALSXTFootBone::Left};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Sound", Meta = (AllowPrivateAccess))
	bool bSkipEffectsWhenInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Sound", Meta = (AllowPrivateAccess))
	bool bSpawnSound{ true };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Sound", Meta = (AllowPrivateAccess, ClampMin = 0, ForceUnits = "x"))
	float SoundVolumeMultiplier{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Sound", Meta = (AllowPrivateAccess, ClampMin = 0, ForceUnits = "x"))
	float SoundPitchMultiplier{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Sound", Meta = (AllowPrivateAccess))
	EALSXTFootstepSoundType SoundType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Sound", Meta = (AllowPrivateAccess))
	bool bIgnoreFootstepSoundBlockCurve;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Decal", Meta = (AllowPrivateAccess))
	bool bSpawnDecal{ true };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Particle System", Meta = (AllowPrivateAccess))
	bool bSpawnParticleSystem{ true };

	UPROPERTY(BlueprintReadOnly, Category = "Settings|Particle System", Meta = (AllowPrivateAccess))
	TWeakObjectPtr<UPhysicalMaterial> LeftFootstepPhysicalMaterial;

	UPROPERTY(BlueprintReadOnly, Category = "Settings|Particle System", Meta = (AllowPrivateAccess))
	TWeakObjectPtr<UPhysicalMaterial> PrevLeftFootstepPhysicalMaterial;

	UPROPERTY(BlueprintReadOnly, Category = "Settings|Particle System", Meta = (AllowPrivateAccess))
	TWeakObjectPtr<UPhysicalMaterial> RightFootstepPhysicalMaterial;

	UPROPERTY(BlueprintReadOnly, Category = "Settings|Particle System", Meta = (AllowPrivateAccess))
	TWeakObjectPtr<UPhysicalMaterial> PrevRightFootstepPhysicalMaterial;

	UPROPERTY(BlueprintReadOnly, Category = "Settings|Values", Meta = (AllowPrivateAccess, ClampMin = 0))
	UTexture2D* LeftFootstepSoleTexture;
	
	UPROPERTY(BlueprintReadOnly, Category = "Settings|Values", Meta = (AllowPrivateAccess, ClampMin = 0))
	UTexture2D* RightFootstepSoleTexture;
	
	UPROPERTY(BlueprintReadOnly, Category = "Settings|Values", Meta = (AllowPrivateAccess, ClampMin = 0))
	UTexture2D* LeftFootstepSoleNormal;
	
	UPROPERTY(BlueprintReadOnly, Category = "Settings|Values", Meta = (AllowPrivateAccess, ClampMin = 0))
	UTexture2D* RightFootstepSoleNormal;
	
	UPROPERTY(BlueprintReadOnly, Category = "Settings|Values", Meta = (AllowPrivateAccess, ClampMin = 0))
	UTexture2D* LeftFootstepSoleDetail;
	
	UPROPERTY(BlueprintReadOnly, Category = "Settings|Values", Meta = (AllowPrivateAccess, ClampMin = 0))
	UTexture2D* RightFootstepSoleDetail;
	
	UPROPERTY(BlueprintReadOnly, Category = "Settings|Values", Meta = (AllowPrivateAccess, ClampMin = 0))
	float LeftFootstepSoleDetailScale{ 0.0f };
	
	UPROPERTY(BlueprintReadOnly, Category = "Settings|Values", Meta = (AllowPrivateAccess, ClampMin = 0))
	float RightFootstepSoleDetailScale{ 0.0f };
	
	UPROPERTY(BlueprintReadOnly, Category = "Settings|Values", Meta = (AllowPrivateAccess, ClampMin = 0))
	FLinearColor LeftFootstepMaterialPrimaryColor{0.0f, 0.0f, 0.0f, 1.0f};
	
	UPROPERTY(BlueprintReadOnly, Category = "Settings|Values", Meta = (AllowPrivateAccess, ClampMin = 0))
	FLinearColor RightFootstepMaterialPrimaryColor{0.0f, 0.0f, 0.0f, 1.0f};
	
	UPROPERTY(BlueprintReadOnly, Category = "Settings|Values", Meta = (AllowPrivateAccess, ClampMin = 0))
	FLinearColor LeftFootstepMaterialSecondaryColor{0.0f, 0.0f, 0.0f, 1.0f};
	
	UPROPERTY(BlueprintReadOnly, Category = "Settings|Values", Meta = (AllowPrivateAccess, ClampMin = 0))
	FLinearColor RightFootstepMaterialSecondaryColor{0.0f, 0.0f, 0.0f, 1.0f};
	
	UPROPERTY(BlueprintReadOnly, Category = "Settings|Values", Meta = (AllowPrivateAccess, ClampMin = 0))
	float LeftFootstepMaterialGrainSize{ 1.0f };
	
	UPROPERTY(BlueprintReadOnly, Category = "Settings|Values", Meta = (AllowPrivateAccess, ClampMin = 0))
	float RightFootstepMaterialGrainSize{ 1.0f };
	
	UPROPERTY(BlueprintReadOnly, Category = "Settings|Values", Meta = (AllowPrivateAccess, ClampMin = 0))
	float LeftFootstepMaterialWetness{ 0.0f };
	
	UPROPERTY(BlueprintReadOnly, Category = "Settings|Values", Meta = (AllowPrivateAccess, ClampMin = 0))
	float RightFootstepMaterialWetness{ 0.0f };
	
	UPROPERTY(BlueprintReadOnly, Category = "Settings|Values", Meta = (AllowPrivateAccess, ClampMin = 0))
	float LeftFootstepMaterialTransferAmount{ 0.5f };
	
	UPROPERTY(BlueprintReadOnly, Category = "Settings|Values", Meta = (AllowPrivateAccess, ClampMin = 0))
	float RightFootstepMaterialTransferAmount{ 0.5f };
	
	UPROPERTY(BlueprintReadOnly, Category = "Settings|Values", Meta = (AllowPrivateAccess, ClampMin = 0))
	float LeftFootstepDecalNormalAmount{0.0f};
	
	UPROPERTY(BlueprintReadOnly, Category = "Settings|Values", Meta = (AllowPrivateAccess, ClampMin = 0))
	float RightFootstepDecalNormalAmount{0.0f};

	UPROPERTY(BlueprintReadOnly, Category = "Settings|Values", Meta = (AllowPrivateAccess, ClampMin = 0))
	float LeftFootstepDecalEmissiveAmount{0.0f};
	
	UPROPERTY(BlueprintReadOnly, Category = "Settings|Values", Meta = (AllowPrivateAccess, ClampMin = 0))
	float RightFootstepDecalEmissiveAmount{0.0f};
	
	UPROPERTY(BlueprintReadOnly, Category = "Settings|Values", Meta = (AllowPrivateAccess, ClampMin = 0))
	float LeftFootstepDecalDuration{4.0f};
	
	UPROPERTY(BlueprintReadOnly, Category = "Settings|Values", Meta = (AllowPrivateAccess, ClampMin = 0))
	float RightFootstepDecalDuration{4.0f};
	
	UPROPERTY(BlueprintReadOnly, Category = "Settings|Values", Meta = (AllowPrivateAccess, ClampMin = 0))
	float LeftFootstepDecalFadeOutDuration{2.0f};
	
	UPROPERTY(BlueprintReadOnly, Category = "Settings|Values", Meta = (AllowPrivateAccess, ClampMin = 0))
	float RightFootstepDecalFadeOutDuration{2.0f};

	UPROPERTY(BlueprintReadOnly, Category = "Settings|Values", Meta = (AllowPrivateAccess))
	float LeftFootstepDurationModifierMin{ 1.0f };

	UPROPERTY(BlueprintReadOnly, Category = "Settings|Values", Meta = (AllowPrivateAccess))
	float RightFootstepDurationModifierMin{ 1.0f };

	UPROPERTY(BlueprintReadOnly, Category = "Settings|Values", Meta = (AllowPrivateAccess))
	float LeftFootstepDurationModifierMax{ 4.0f };

	UPROPERTY(BlueprintReadOnly, Category = "Settings|Values", Meta = (AllowPrivateAccess))
	float RightFootstepDurationModifierMax{ 4.0f };

	UPROPERTY(BlueprintReadOnly, Category = "Settings|Values", Meta = (AllowPrivateAccess))
	TWeakObjectPtr<UPhysicalMaterial> FootstepSoleSurfaceType;

public:
	virtual FString GetNotifyName_Implementation() const override;

	virtual void Notify(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(Meta = (AllowPrivateAccess))
	FHitResult HitResult;
};
